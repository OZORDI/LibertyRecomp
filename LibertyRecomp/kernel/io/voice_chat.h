#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <unordered_map>
#include <mutex>
#include <atomic>
#include <functional>
#include <memory>
#include <queue>
#include <chrono>

struct SDL_AudioSpec;

namespace Net {

// Forward declarations
class P2PManager;

// ============================================================================
// Voice Chat Constants
// ============================================================================

// Xbox 360 voice uses 8kHz 16-bit mono PCM, compressed with MS-ADPCM or similar
// We'll use Opus codec for better quality at low bitrates over the network

constexpr int VOICE_SAMPLE_RATE = 16000;          // 16kHz for better quality
constexpr int VOICE_CHANNELS = 1;                  // Mono
constexpr int VOICE_BITS_PER_SAMPLE = 16;          // 16-bit
constexpr int VOICE_FRAME_SIZE_MS = 20;            // 20ms frames (standard for voice)
constexpr int VOICE_FRAME_SAMPLES = (VOICE_SAMPLE_RATE * VOICE_FRAME_SIZE_MS) / 1000;  // 320 samples
constexpr int VOICE_FRAME_BYTES = VOICE_FRAME_SAMPLES * sizeof(int16_t);  // 640 bytes raw

// Opus typically compresses voice to ~32 kbps = 4KB/s = ~80 bytes per 20ms frame
constexpr int VOICE_MAX_PACKET_SIZE = 256;         // Max compressed packet size
constexpr int VOICE_JITTER_BUFFER_MS = 100;        // Jitter buffer size in ms
constexpr int VOICE_JITTER_BUFFER_FRAMES = VOICE_JITTER_BUFFER_MS / VOICE_FRAME_SIZE_MS;

// Voice packet header for network transmission
constexpr uint8_t VOICE_PACKET_MAGIC = 0x56;       // 'V' for Voice Chat magic byte
constexpr uint8_t VOICE_PACKET_VERSION = 1;

// ============================================================================
// Voice Packet Structure
// ============================================================================

#pragma pack(push, 1)

struct VoicePacketHeader {
    uint8_t magic;              // VOICE_PACKET_MAGIC
    uint8_t version;            // Protocol version
    uint16_t sequenceNumber;    // For ordering/loss detection
    uint32_t timestamp;         // RTP-style timestamp (samples)
    uint16_t payloadSize;       // Size of compressed audio data
    uint8_t flags;              // Voice activity, mute status, etc.
    uint8_t reserved;           // Padding
};

#pragma pack(pop)

// Voice packet flags
enum VoicePacketFlags : uint8_t {
    VOICE_FLAG_NONE = 0,
    VOICE_FLAG_VOICE_ACTIVITY = 0x01,    // Voice activity detected
    VOICE_FLAG_MUTED = 0x02,             // Sender is muted
    VOICE_FLAG_KEY_FRAME = 0x04,         // Key frame for codec
};

// ============================================================================
// Voice Channel State
// ============================================================================

// State for a single peer's voice playback
struct VoicePeerState {
    uint32_t peerId;                     // Virtual IP of peer
    std::string displayName;
    
    // Jitter buffer
    struct JitterFrame {
        uint16_t sequenceNumber;
        uint32_t timestamp;
        std::vector<int16_t> samples;
        bool used;
    };
    std::vector<JitterFrame> jitterBuffer;
    uint16_t lastPlayedSequence;
    uint32_t lastPlayedTimestamp;
    
    // Playback state
    std::vector<int16_t> playbackBuffer;
    size_t playbackReadPos;
    
    // Statistics
    uint32_t packetsReceived;
    uint32_t packetsLost;
    uint32_t lastPacketTime;             // For timeout detection
    
    // User settings
    float volume;                         // 0.0 - 1.0
    bool muted;                           // Locally muted by user
    bool speaking;                        // Currently has voice activity
    
    VoicePeerState()
        : peerId(0), lastPlayedSequence(0), lastPlayedTimestamp(0),
          playbackReadPos(0), packetsReceived(0), packetsLost(0),
          lastPacketTime(0), volume(1.0f), muted(false), speaking(false) {
        jitterBuffer.resize(VOICE_JITTER_BUFFER_FRAMES);
    }
};

// ============================================================================
// Voice Chat Manager
// ============================================================================

class VoiceChatManager {
public:
    static VoiceChatManager& Instance();
    
    // =========================================================================
    // Initialization
    // =========================================================================
    
    /**
     * Initialize the voice chat system
     * Sets up SDL audio capture and playback devices
     * @return true on success
     */
    bool Initialize();
    
    /**
     * Shutdown voice chat
     */
    void Shutdown();
    
    /**
     * Check if voice chat is initialized
     */
    bool IsInitialized() const { return initialized_.load(); }
    
    /**
     * Check if a headset/microphone is available
     */
    bool IsHeadsetPresent() const { return headsetPresent_.load(); }
    
    // =========================================================================
    // XamVoice API (Called from kernel imports)
    // =========================================================================
    
    /**
     * Create a voice channel for a user (Xbox API)
     * @param userIndex Xbox user index (0-3)
     * @param flags Creation flags
     * @param outVoice Output voice handle
     * @return Error code (0 = success)
     */
    uint32_t CreateVoiceChannel(uint32_t userIndex, uint32_t flags, void** outVoice);
    
    /**
     * Close a voice channel
     * @param voice Voice handle from CreateVoiceChannel
     */
    void CloseVoiceChannel(void* voice);
    
    /**
     * Submit a voice packet from the game
     * This is called when the game wants to send voice data
     * @param voice Voice handle
     * @param size Packet size
     * @param data Voice data
     * @return Error code
     */
    uint32_t SubmitVoicePacket(void* voice, uint32_t size, void* data);
    
    /**
     * Get received voice packet for the game
     * @param voice Voice handle
     * @param outSize Output packet size
     * @param outData Output buffer
     * @param bufferSize Size of output buffer
     * @return Error code
     */
    uint32_t GetVoicePacket(void* voice, uint32_t* outSize, void* outData, uint32_t bufferSize);
    
    // =========================================================================
    // Voice Activity
    // =========================================================================
    
    /**
     * Check if local user is currently talking
     */
    bool IsLocalTalking() const { return localTalking_.load(); }
    
    /**
     * Check if a remote peer is talking
     * @param peerId Virtual IP of peer
     */
    bool IsPeerTalking(uint32_t peerId) const;
    
    /**
     * Get list of currently talking peers
     */
    std::vector<uint32_t> GetTalkingPeers() const;
    
    // =========================================================================
    // Settings
    // =========================================================================
    
    /**
     * Enable/disable voice chat
     */
    void SetEnabled(bool enabled);
    bool IsEnabled() const { return enabled_.load(); }
    
    /**
     * Set microphone volume (0.0 - 1.0)
     */
    void SetMicrophoneVolume(float volume);
    float GetMicrophoneVolume() const { return micVolume_.load(); }
    
    /**
     * Set output volume (0.0 - 1.0)
     */
    void SetOutputVolume(float volume);
    float GetOutputVolume() const { return outputVolume_.load(); }
    
    /**
     * Mute/unmute self
     */
    void SetSelfMuted(bool muted);
    bool IsSelfMuted() const { return selfMuted_.load(); }
    
    /**
     * Mute a specific peer
     */
    void SetPeerMuted(uint32_t peerId, bool muted);
    bool IsPeerMuted(uint32_t peerId) const;
    
    /**
     * Set push-to-talk mode
     */
    void SetPushToTalk(bool enabled);
    bool IsPushToTalk() const { return pushToTalk_.load(); }
    
    /**
     * Set push-to-talk key state
     */
    void SetPushToTalkActive(bool active);
    
    /**
     * Set voice activity detection threshold (0.0 - 1.0)
     */
    void SetVoiceActivityThreshold(float threshold);
    
    // =========================================================================
    // Network Integration
    // =========================================================================
    
    /**
     * Process incoming voice data from P2P network
     * Called by P2PManager when voice packets arrive
     */
    void OnVoiceDataReceived(uint32_t peerId, const void* data, size_t size);
    
    /**
     * Called when a peer connects
     */
    void OnPeerConnected(uint32_t peerId, const std::string& displayName);
    
    /**
     * Called when a peer disconnects
     */
    void OnPeerDisconnected(uint32_t peerId);
    
    // =========================================================================
    // Processing (Call from main loop)
    // =========================================================================
    
    /**
     * Process voice chat - capture, encode, decode, playback
     * Should be called regularly (e.g., every frame)
     */
    void Poll();
    
private:
    VoiceChatManager();
    ~VoiceChatManager();
    
    // Prevent copying
    VoiceChatManager(const VoiceChatManager&) = delete;
    VoiceChatManager& operator=(const VoiceChatManager&) = delete;
    
    // Audio device management
    bool InitializeCaptureDevice();
    bool InitializePlaybackDevice();
    void CloseCaptureDevice();
    void ClosePlaybackDevice();
    
    // Audio processing
    void ProcessCapture();
    void ProcessPlayback();
    void MixPeerAudio(int16_t* output, int samples);
    
    // Encoding/Decoding (simple for now, can add Opus later)
    void EncodeAndSendFrame(const int16_t* samples, int sampleCount);
    void DecodeAndQueueFrame(uint32_t peerId, const VoicePacketHeader* header, 
                             const uint8_t* payload, size_t payloadSize);
    
    // Voice activity detection
    bool DetectVoiceActivity(const int16_t* samples, int sampleCount);
    
    // State
    std::atomic<bool> initialized_{false};
    std::atomic<bool> enabled_{true};
    std::atomic<bool> headsetPresent_{false};
    
    // Voice channels (Xbox API)
    struct VoiceChannel {
        uint32_t userIndex;
        uint32_t flags;
        bool active;
        std::queue<std::vector<uint8_t>> receiveQueue;  // Packets from peers
    };
    std::mutex channelsMutex_;
    std::unordered_map<void*, std::unique_ptr<VoiceChannel>> channels_;
    void* nextChannelHandle_{reinterpret_cast<void*>(1)};
    
    // Peer states
    mutable std::mutex peersMutex_;
    std::unordered_map<uint32_t, std::unique_ptr<VoicePeerState>> peers_;
    
    // Capture state
    uint32_t captureDeviceId_{0};
    std::vector<int16_t> captureBuffer_;
    std::mutex captureMutex_;
    std::atomic<bool> capturing_{false};
    uint16_t sendSequenceNumber_{0};
    uint32_t sendTimestamp_{0};
    
    // Playback state
    uint32_t playbackDeviceId_{0};
    std::vector<int16_t> playbackBuffer_;
    std::mutex playbackMutex_;
    std::atomic<bool> playing_{false};
    
    // Settings
    std::atomic<float> micVolume_{1.0f};
    std::atomic<float> outputVolume_{1.0f};
    std::atomic<bool> selfMuted_{false};
    std::atomic<bool> pushToTalk_{false};
    std::atomic<bool> pushToTalkActive_{false};
    std::atomic<float> voiceActivityThreshold_{0.02f};  // 2% of max amplitude
    
    // Status
    std::atomic<bool> localTalking_{false};
    
    // Timing
    std::chrono::steady_clock::time_point lastPollTime_;
};

// ============================================================================
// Helper Functions
// ============================================================================

// Check if a network packet is a voice packet
inline bool IsVoicePacket(const void* data, size_t size) {
    if (size < sizeof(VoicePacketHeader)) return false;
    const auto* header = static_cast<const VoicePacketHeader*>(data);
    return header->magic == VOICE_PACKET_MAGIC && header->version == VOICE_PACKET_VERSION;
}

} // namespace Net
