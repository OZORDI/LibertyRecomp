#include "voice_chat.h"
#include "p2p_manager.h"
#include <os/logger.h>
#include <user/config.h>
#include <SDL.h>
#include <cstring>
#include <cmath>
#include <algorithm>

namespace Net {

// ============================================================================
// SDL Audio Callbacks
// ============================================================================

static void SDLCALL AudioCaptureCallback(void* userdata, Uint8* stream, int len) {
    auto* manager = static_cast<VoiceChatManager*>(userdata);
    if (!manager) return;
    
    // Forward to manager - it will handle the captured audio
    const int16_t* samples = reinterpret_cast<const int16_t*>(stream);
    int sampleCount = len / sizeof(int16_t);
    
    // Copy to capture buffer (thread-safe via mutex in ProcessCapture)
    // The manager will process this in Poll()
}

static void SDLCALL AudioPlaybackCallback(void* userdata, Uint8* stream, int len) {
    auto* manager = static_cast<VoiceChatManager*>(userdata);
    if (!manager) return;
    
    // Clear output buffer first
    std::memset(stream, 0, len);
    
    // Mix peer audio into the output
    int16_t* output = reinterpret_cast<int16_t*>(stream);
    int samples = len / sizeof(int16_t);
    
    // Manager handles mixing from all peer jitter buffers
}

// ============================================================================
// VoiceChatManager Implementation
// ============================================================================

VoiceChatManager& VoiceChatManager::Instance() {
    static VoiceChatManager instance;
    return instance;
}

VoiceChatManager::VoiceChatManager() {
    captureBuffer_.resize(VOICE_FRAME_SAMPLES * 4);  // Buffer for multiple frames
    playbackBuffer_.resize(VOICE_FRAME_SAMPLES * 4);
}

VoiceChatManager::~VoiceChatManager() {
    Shutdown();
}

bool VoiceChatManager::Initialize() {
    if (initialized_.load()) {
        return true;
    }
    
    LOG_WARNING("[VoiceChat] Initializing voice chat system...");
    
    // Ensure SDL audio subsystem is initialized
    if (!(SDL_WasInit(SDL_INIT_AUDIO) & SDL_INIT_AUDIO)) {
        if (SDL_InitSubSystem(SDL_INIT_AUDIO) < 0) {
            LOGF_ERROR("[VoiceChat] Failed to initialize SDL audio: {}", SDL_GetError());
            return false;
        }
    }
    
    // List available capture devices
    int captureDeviceCount = SDL_GetNumAudioDevices(SDL_TRUE);
    LOGF_WARNING("[VoiceChat] Found {} capture devices:", captureDeviceCount);
    for (int i = 0; i < captureDeviceCount; ++i) {
        LOGF_WARNING("[VoiceChat]   {}: {}", i, SDL_GetAudioDeviceName(i, SDL_TRUE));
    }
    
    // Check for microphone availability
    headsetPresent_.store(captureDeviceCount > 0);
    
    if (headsetPresent_.load()) {
        if (!InitializeCaptureDevice()) {
            LOG_WARNING("[VoiceChat] Failed to initialize capture device, voice chat will be receive-only");
        }
    } else {
        LOG_WARNING("[VoiceChat] No microphone detected, voice chat will be receive-only");
    }
    
    // Initialize playback device
    if (!InitializePlaybackDevice()) {
        LOG_WARNING("[VoiceChat] Failed to initialize playback device, voice chat disabled");
        CloseCaptureDevice();
        return false;
    }
    
    initialized_.store(true);
    lastPollTime_ = std::chrono::steady_clock::now();
    
    // Apply config settings
    enabled_.store(Config::VoiceChatEnabled.Value);
    micVolume_.store(Config::MicrophoneVolume.Value);
    outputVolume_.store(Config::VoiceOutputVolume.Value);
    pushToTalk_.store(Config::PushToTalk.Value);
    voiceActivityThreshold_.store(Config::VoiceActivityThreshold.Value);
    selfMuted_.store(Config::VoiceChatSelfMuted.Value);
    
    LOG_WARNING("[VoiceChat] Voice chat initialized successfully");
    return true;
}

void VoiceChatManager::Shutdown() {
    if (!initialized_.load()) {
        return;
    }
    
    LOG_WARNING("[VoiceChat] Shutting down voice chat...");
    
    CloseCaptureDevice();
    ClosePlaybackDevice();
    
    // Clear peer states
    {
        std::lock_guard<std::mutex> lock(peersMutex_);
        peers_.clear();
    }
    
    // Clear voice channels
    {
        std::lock_guard<std::mutex> lock(channelsMutex_);
        channels_.clear();
    }
    
    initialized_.store(false);
    LOG_WARNING("[VoiceChat] Voice chat shutdown complete");
}

bool VoiceChatManager::InitializeCaptureDevice() {
    SDL_AudioSpec desired, obtained;
    
    SDL_zero(desired);
    desired.freq = VOICE_SAMPLE_RATE;
    desired.format = AUDIO_S16SYS;
    desired.channels = VOICE_CHANNELS;
    desired.samples = VOICE_FRAME_SAMPLES;
    desired.callback = nullptr;  // We'll use SDL_DequeueAudio instead of callback
    desired.userdata = this;
    
    // Open default capture device
    captureDeviceId_ = SDL_OpenAudioDevice(nullptr, SDL_TRUE, &desired, &obtained, 0);
    
    if (captureDeviceId_ == 0) {
        LOGF_ERROR("[VoiceChat] Failed to open capture device: {}", SDL_GetError());
        return false;
    }
    
    LOGF_WARNING("[VoiceChat] Capture device opened: freq={}, channels={}, samples={}",
                obtained.freq, obtained.channels, obtained.samples);
    
    // Start capturing
    SDL_PauseAudioDevice(captureDeviceId_, 0);
    capturing_.store(true);
    
    return true;
}

bool VoiceChatManager::InitializePlaybackDevice() {
    SDL_AudioSpec desired, obtained;
    
    SDL_zero(desired);
    desired.freq = VOICE_SAMPLE_RATE;
    desired.format = AUDIO_S16SYS;
    desired.channels = VOICE_CHANNELS;
    desired.samples = VOICE_FRAME_SAMPLES;
    desired.callback = nullptr;  // We'll use SDL_QueueAudio
    desired.userdata = this;
    
    // Open default playback device for voice
    // Note: This is separate from game audio
    playbackDeviceId_ = SDL_OpenAudioDevice(nullptr, SDL_FALSE, &desired, &obtained, 0);
    
    if (playbackDeviceId_ == 0) {
        LOGF_ERROR("[VoiceChat] Failed to open playback device: {}", SDL_GetError());
        return false;
    }
    
    LOGF_WARNING("[VoiceChat] Playback device opened: freq={}, channels={}, samples={}",
                obtained.freq, obtained.channels, obtained.samples);
    
    // Start playback
    SDL_PauseAudioDevice(playbackDeviceId_, 0);
    playing_.store(true);
    
    return true;
}

void VoiceChatManager::CloseCaptureDevice() {
    if (captureDeviceId_ != 0) {
        SDL_CloseAudioDevice(captureDeviceId_);
        captureDeviceId_ = 0;
        capturing_.store(false);
        LOG_WARNING("[VoiceChat] Capture device closed");
    }
}

void VoiceChatManager::ClosePlaybackDevice() {
    if (playbackDeviceId_ != 0) {
        SDL_CloseAudioDevice(playbackDeviceId_);
        playbackDeviceId_ = 0;
        playing_.store(false);
        LOG_WARNING("[VoiceChat] Playback device closed");
    }
}

// ============================================================================
// XamVoice API Implementation
// ============================================================================

uint32_t VoiceChatManager::CreateVoiceChannel(uint32_t userIndex, uint32_t flags, void** outVoice) {
    if (!initialized_.load()) {
        // Auto-initialize if not already done
        if (!Initialize()) {
            if (outVoice) *outVoice = nullptr;
            return 0x8007048F;  // ERROR_DEVICE_NOT_CONNECTED
        }
    }
    
    if (!outVoice) {
        return 0x80070057;  // E_INVALIDARG
    }
    
    // Check if headset is present
    if (!headsetPresent_.load()) {
        *outVoice = nullptr;
        return 0x8007048F;  // ERROR_DEVICE_NOT_CONNECTED
    }
    
    std::lock_guard<std::mutex> lock(channelsMutex_);
    
    auto channel = std::make_unique<VoiceChannel>();
    channel->userIndex = userIndex;
    channel->flags = flags;
    channel->active = true;
    
    void* handle = nextChannelHandle_;
    nextChannelHandle_ = reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(nextChannelHandle_) + 1);
    
    channels_[handle] = std::move(channel);
    *outVoice = handle;
    
    LOGF_WARNING("[VoiceChat] Created voice channel for user {} -> handle={}", userIndex, handle);
    
    return 0;  // ERROR_SUCCESS
}

void VoiceChatManager::CloseVoiceChannel(void* voice) {
    if (!voice) return;
    
    std::lock_guard<std::mutex> lock(channelsMutex_);
    
    auto it = channels_.find(voice);
    if (it != channels_.end()) {
        LOGF_WARNING("[VoiceChat] Closing voice channel handle={}", voice);
        channels_.erase(it);
    }
}

uint32_t VoiceChatManager::SubmitVoicePacket(void* voice, uint32_t size, void* data) {
    if (!voice || !data || size == 0) {
        return 0x80070057;  // E_INVALIDARG
    }
    
    if (!enabled_.load() || selfMuted_.load()) {
        return 0;  // Silently succeed but don't send
    }
    
    if (pushToTalk_.load() && !pushToTalkActive_.load()) {
        return 0;  // Push-to-talk not active
    }
    
    // The game is giving us voice data it wants to transmit
    // For GTA IV, this is likely already encoded audio from the Xbox voice codec
    // We'll re-encode it for our network transport
    
    // For now, treat the input as raw PCM and send it
    // In a full implementation, we'd decode the Xbox format first
    
    const int16_t* samples = static_cast<const int16_t*>(data);
    int sampleCount = size / sizeof(int16_t);
    
    // Check for voice activity
    if (!DetectVoiceActivity(samples, sampleCount)) {
        localTalking_.store(false);
        return 0;  // No voice activity, don't send
    }
    
    localTalking_.store(true);
    
    // Send to all peers via P2P
    EncodeAndSendFrame(samples, sampleCount);
    
    return 0;  // ERROR_SUCCESS
}

uint32_t VoiceChatManager::GetVoicePacket(void* voice, uint32_t* outSize, void* outData, uint32_t bufferSize) {
    if (!voice || !outSize || !outData) {
        return 0x80070057;  // E_INVALIDARG
    }
    
    std::lock_guard<std::mutex> lock(channelsMutex_);
    
    auto it = channels_.find(voice);
    if (it == channels_.end() || !it->second->active) {
        *outSize = 0;
        return 0x80070490;  // ERROR_NOT_FOUND
    }
    
    auto& channel = it->second;
    
    if (channel->receiveQueue.empty()) {
        *outSize = 0;
        return 0;  // No data available
    }
    
    auto& packet = channel->receiveQueue.front();
    
    if (packet.size() > bufferSize) {
        *outSize = 0;
        return 0x8007007A;  // ERROR_INSUFFICIENT_BUFFER
    }
    
    std::memcpy(outData, packet.data(), packet.size());
    *outSize = static_cast<uint32_t>(packet.size());
    channel->receiveQueue.pop();
    
    return 0;  // ERROR_SUCCESS
}

// ============================================================================
// Voice Activity
// ============================================================================

bool VoiceChatManager::IsPeerTalking(uint32_t peerId) const {
    std::lock_guard<std::mutex> lock(peersMutex_);
    
    auto it = peers_.find(peerId);
    if (it != peers_.end()) {
        return it->second->speaking;
    }
    return false;
}

std::vector<uint32_t> VoiceChatManager::GetTalkingPeers() const {
    std::vector<uint32_t> result;
    
    std::lock_guard<std::mutex> lock(peersMutex_);
    
    for (const auto& pair : peers_) {
        if (pair.second->speaking) {
            result.push_back(pair.first);
        }
    }
    
    return result;
}

bool VoiceChatManager::DetectVoiceActivity(const int16_t* samples, int sampleCount) {
    if (sampleCount <= 0) return false;
    
    // Simple energy-based voice activity detection
    float threshold = voiceActivityThreshold_.load();
    int64_t energy = 0;
    
    for (int i = 0; i < sampleCount; ++i) {
        energy += static_cast<int64_t>(samples[i]) * samples[i];
    }
    
    float rms = std::sqrt(static_cast<float>(energy) / sampleCount);
    float normalizedRms = rms / 32768.0f;  // Normalize to 0-1 range
    
    return normalizedRms > threshold;
}

// ============================================================================
// Settings
// ============================================================================

void VoiceChatManager::SetEnabled(bool enabled) {
    enabled_.store(enabled);
    LOGF_WARNING("[VoiceChat] Voice chat {}", enabled ? "enabled" : "disabled");
}

void VoiceChatManager::SetMicrophoneVolume(float volume) {
    micVolume_.store(std::clamp(volume, 0.0f, 1.0f));
}

void VoiceChatManager::SetOutputVolume(float volume) {
    outputVolume_.store(std::clamp(volume, 0.0f, 1.0f));
}

void VoiceChatManager::SetSelfMuted(bool muted) {
    selfMuted_.store(muted);
    LOGF_WARNING("[VoiceChat] Self-mute {}", muted ? "on" : "off");
}

void VoiceChatManager::SetPeerMuted(uint32_t peerId, bool muted) {
    std::lock_guard<std::mutex> lock(peersMutex_);
    
    auto it = peers_.find(peerId);
    if (it != peers_.end()) {
        it->second->muted = muted;
        LOGF_WARNING("[VoiceChat] Peer {} {}", peerId, muted ? "muted" : "unmuted");
    }
}

bool VoiceChatManager::IsPeerMuted(uint32_t peerId) const {
    std::lock_guard<std::mutex> lock(peersMutex_);
    
    auto it = peers_.find(peerId);
    if (it != peers_.end()) {
        return it->second->muted;
    }
    return false;
}

void VoiceChatManager::SetPushToTalk(bool enabled) {
    pushToTalk_.store(enabled);
    LOGF_WARNING("[VoiceChat] Push-to-talk {}", enabled ? "enabled" : "disabled");
}

void VoiceChatManager::SetPushToTalkActive(bool active) {
    pushToTalkActive_.store(active);
}

void VoiceChatManager::SetVoiceActivityThreshold(float threshold) {
    voiceActivityThreshold_.store(std::clamp(threshold, 0.0f, 1.0f));
}

// ============================================================================
// Network Integration
// ============================================================================

void VoiceChatManager::OnVoiceDataReceived(uint32_t peerId, const void* data, size_t size) {
    if (!initialized_.load() || !enabled_.load()) {
        return;
    }
    
    if (size < sizeof(VoicePacketHeader)) {
        return;
    }
    
    const auto* header = static_cast<const VoicePacketHeader*>(data);
    
    if (header->magic != VOICE_PACKET_MAGIC || header->version != VOICE_PACKET_VERSION) {
        return;  // Not a voice packet
    }
    
    size_t payloadSize = header->payloadSize;
    if (sizeof(VoicePacketHeader) + payloadSize > size) {
        return;  // Invalid packet
    }
    
    const uint8_t* payload = static_cast<const uint8_t*>(data) + sizeof(VoicePacketHeader);
    
    // Decode and queue for playback
    DecodeAndQueueFrame(peerId, header, payload, payloadSize);
}

void VoiceChatManager::OnPeerConnected(uint32_t peerId, const std::string& displayName) {
    std::lock_guard<std::mutex> lock(peersMutex_);
    
    auto state = std::make_unique<VoicePeerState>();
    state->peerId = peerId;
    state->displayName = displayName;
    
    peers_[peerId] = std::move(state);
    
    LOGF_WARNING("[VoiceChat] Peer connected: {} ({})", displayName, peerId);
}

void VoiceChatManager::OnPeerDisconnected(uint32_t peerId) {
    std::lock_guard<std::mutex> lock(peersMutex_);
    
    auto it = peers_.find(peerId);
    if (it != peers_.end()) {
        LOGF_WARNING("[VoiceChat] Peer disconnected: {} ({})", it->second->displayName, peerId);
        peers_.erase(it);
    }
}

// ============================================================================
// Processing
// ============================================================================

void VoiceChatManager::Poll() {
    if (!initialized_.load() || !enabled_.load()) {
        return;
    }
    
    auto now = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastPollTime_);
    
    // Only process at voice frame rate
    if (elapsed.count() < VOICE_FRAME_SIZE_MS / 2) {
        return;
    }
    lastPollTime_ = now;
    
    ProcessCapture();
    ProcessPlayback();
}

void VoiceChatManager::ProcessCapture() {
    if (!capturing_.load() || captureDeviceId_ == 0) {
        return;
    }
    
    if (selfMuted_.load()) {
        // Discard captured audio
        SDL_ClearQueuedAudio(captureDeviceId_);
        localTalking_.store(false);
        return;
    }
    
    if (pushToTalk_.load() && !pushToTalkActive_.load()) {
        // Push-to-talk not active, discard audio
        SDL_ClearQueuedAudio(captureDeviceId_);
        localTalking_.store(false);
        return;
    }
    
    // Check how much audio is available
    Uint32 available = SDL_GetQueuedAudioSize(captureDeviceId_);
    Uint32 frameBytes = VOICE_FRAME_BYTES;
    
    if (available < frameBytes) {
        return;  // Not enough data for a frame
    }
    
    // Read captured audio
    std::vector<int16_t> frame(VOICE_FRAME_SAMPLES);
    Uint32 dequeued = SDL_DequeueAudio(captureDeviceId_, frame.data(), frameBytes);
    
    if (dequeued < frameBytes) {
        return;
    }
    
    // Apply microphone volume
    float volume = micVolume_.load();
    if (volume < 0.99f) {
        for (auto& sample : frame) {
            sample = static_cast<int16_t>(sample * volume);
        }
    }
    
    // Check for voice activity
    if (!DetectVoiceActivity(frame.data(), VOICE_FRAME_SAMPLES)) {
        localTalking_.store(false);
        return;
    }
    
    localTalking_.store(true);
    
    // Encode and send
    EncodeAndSendFrame(frame.data(), VOICE_FRAME_SAMPLES);
}

void VoiceChatManager::ProcessPlayback() {
    if (!playing_.load() || playbackDeviceId_ == 0) {
        return;
    }
    
    // Mix audio from all peers into output buffer
    std::vector<int16_t> mixBuffer(VOICE_FRAME_SAMPLES, 0);
    
    MixPeerAudio(mixBuffer.data(), VOICE_FRAME_SAMPLES);
    
    // Apply output volume
    float volume = outputVolume_.load();
    if (volume < 0.99f) {
        for (auto& sample : mixBuffer) {
            sample = static_cast<int16_t>(sample * volume);
        }
    }
    
    // Check if there's actually audio to play
    bool hasAudio = false;
    for (auto sample : mixBuffer) {
        if (sample != 0) {
            hasAudio = true;
            break;
        }
    }
    
    if (hasAudio) {
        // Queue for playback
        SDL_QueueAudio(playbackDeviceId_, mixBuffer.data(), VOICE_FRAME_BYTES);
    }
}

void VoiceChatManager::MixPeerAudio(int16_t* output, int samples) {
    std::lock_guard<std::mutex> lock(peersMutex_);
    
    // Mixing buffer (32-bit to avoid clipping)
    std::vector<int32_t> mixBuffer(samples, 0);
    int activePeers = 0;
    
    uint32_t currentTime = SDL_GetTicks();
    
    for (auto& pair : peers_) {
        auto& peer = pair.second;
        
        // Skip muted peers
        if (peer->muted) {
            peer->speaking = false;
            continue;
        }
        
        // Check for timeout (no packets in 500ms = not speaking)
        if (currentTime - peer->lastPacketTime > 500) {
            peer->speaking = false;
            continue;
        }
        
        // Get samples from jitter buffer
        if (peer->playbackBuffer.empty()) {
            continue;
        }
        
        size_t available = peer->playbackBuffer.size() - peer->playbackReadPos;
        size_t toRead = std::min(available, static_cast<size_t>(samples));
        
        if (toRead > 0) {
            float peerVolume = peer->volume;
            
            for (size_t i = 0; i < toRead; ++i) {
                mixBuffer[i] += static_cast<int32_t>(peer->playbackBuffer[peer->playbackReadPos + i] * peerVolume);
            }
            
            peer->playbackReadPos += toRead;
            
            // Remove consumed samples
            if (peer->playbackReadPos >= peer->playbackBuffer.size() / 2) {
                peer->playbackBuffer.erase(
                    peer->playbackBuffer.begin(),
                    peer->playbackBuffer.begin() + peer->playbackReadPos
                );
                peer->playbackReadPos = 0;
            }
            
            activePeers++;
            peer->speaking = true;
        }
    }
    
    // Convert to 16-bit with clipping
    for (int i = 0; i < samples; ++i) {
        int32_t sample = mixBuffer[i];
        if (sample > 32767) sample = 32767;
        if (sample < -32768) sample = -32768;
        output[i] = static_cast<int16_t>(sample);
    }
}

void VoiceChatManager::EncodeAndSendFrame(const int16_t* samples, int sampleCount) {
    if (!P2PManager::Instance().IsInSession()) {
        return;
    }
    
    // For now, send raw PCM (simple implementation)
    // In a production system, we'd use Opus for ~10:1 compression
    
    // Build packet
    std::vector<uint8_t> packet(sizeof(VoicePacketHeader) + sampleCount * sizeof(int16_t));
    
    auto* header = reinterpret_cast<VoicePacketHeader*>(packet.data());
    header->magic = VOICE_PACKET_MAGIC;
    header->version = VOICE_PACKET_VERSION;
    header->sequenceNumber = sendSequenceNumber_++;
    header->timestamp = sendTimestamp_;
    header->payloadSize = static_cast<uint16_t>(sampleCount * sizeof(int16_t));
    header->flags = VOICE_FLAG_VOICE_ACTIVITY;
    header->reserved = 0;
    
    // Copy audio data
    std::memcpy(packet.data() + sizeof(VoicePacketHeader), samples, sampleCount * sizeof(int16_t));
    
    // Update timestamp for next frame
    sendTimestamp_ += sampleCount;
    
    // Send to all peers (unreliable - voice can tolerate loss)
    P2PManager::Instance().Broadcast(packet.data(), packet.size(), false);
}

void VoiceChatManager::DecodeAndQueueFrame(uint32_t peerId, const VoicePacketHeader* header,
                                           const uint8_t* payload, size_t payloadSize) {
    std::lock_guard<std::mutex> lock(peersMutex_);
    
    auto it = peers_.find(peerId);
    if (it == peers_.end()) {
        // Create peer state on-the-fly if we receive voice before P2P notification
        auto state = std::make_unique<VoicePeerState>();
        state->peerId = peerId;
        peers_[peerId] = std::move(state);
        it = peers_.find(peerId);
    }
    
    auto& peer = it->second;
    
    // Update statistics
    peer->packetsReceived++;
    peer->lastPacketTime = SDL_GetTicks();
    
    // Check for lost packets
    uint16_t expectedSeq = peer->lastPlayedSequence + 1;
    if (header->sequenceNumber != expectedSeq && peer->lastPlayedSequence != 0) {
        int lost = static_cast<int>(header->sequenceNumber - expectedSeq);
        if (lost > 0 && lost < 100) {
            peer->packetsLost += lost;
        }
    }
    peer->lastPlayedSequence = header->sequenceNumber;
    
    // Decode (currently raw PCM, just copy)
    size_t sampleCount = payloadSize / sizeof(int16_t);
    const int16_t* samples = reinterpret_cast<const int16_t*>(payload);
    
    // Add to playback buffer
    peer->playbackBuffer.insert(peer->playbackBuffer.end(), samples, samples + sampleCount);
    
    // Limit buffer size to prevent memory growth
    size_t maxBufferSize = VOICE_SAMPLE_RATE * 2;  // 2 seconds max
    if (peer->playbackBuffer.size() > maxBufferSize) {
        peer->playbackBuffer.erase(
            peer->playbackBuffer.begin(),
            peer->playbackBuffer.begin() + (peer->playbackBuffer.size() - maxBufferSize)
        );
        peer->playbackReadPos = 0;
    }
    
    // Also queue for the game's voice API if there are active channels
    {
        std::lock_guard<std::mutex> channelLock(channelsMutex_);
        for (auto& channelPair : channels_) {
            if (channelPair.second->active) {
                std::vector<uint8_t> gamePacket(payload, payload + payloadSize);
                channelPair.second->receiveQueue.push(std::move(gamePacket));
                
                // Limit queue size
                while (channelPair.second->receiveQueue.size() > 50) {
                    channelPair.second->receiveQueue.pop();
                }
            }
        }
    }
}

} // namespace Net
