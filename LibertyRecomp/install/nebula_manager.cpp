#include "nebula_manager.h"
#include "platform_paths.h"
#include <os/logger.h>

#include <fstream>
#include <sstream>
#include <random>
#include <chrono>
#include <cstdlib>
#include <array>

#ifdef _WIN32
#include <windows.h>
#include <tlhelp32.h>
#else
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <spawn.h>
extern char **environ;
#endif

namespace liberty::install {

// ============================================================================
// Singleton Instance
// ============================================================================

NebulaManager& NebulaManager::Instance() {
    static NebulaManager instance;
    return instance;
}

NebulaManager::NebulaManager() {
    // Initialize binary paths
    auto nebulaDir = GetNebulaDirectory();
    
#ifdef _WIN32
    nebulaBinaryPath_ = nebulaDir / "nebula.exe";
    nebulaCertBinaryPath_ = nebulaDir / "nebula-cert.exe";
#else
    nebulaBinaryPath_ = nebulaDir / "nebula";
    nebulaCertBinaryPath_ = nebulaDir / "nebula-cert";
#endif

    // Config templates are in the install directory
    configTemplateDir_ = nebulaDir / "config";
}

NebulaManager::~NebulaManager() {
    // Stop Nebula if running
    if (isRunning_) {
        Stop();
    }
}

// ============================================================================
// Initialization
// ============================================================================

std::filesystem::path NebulaManager::GetNebulaDirectory() {
    return PlatformPaths::GetInstallDirectory() / "nebula";
}

std::filesystem::path NebulaManager::GetNebulaBinaryPath() {
    return nebulaBinaryPath_;
}

std::filesystem::path NebulaManager::GetNebulaCertBinaryPath() {
    return nebulaCertBinaryPath_;
}

bool NebulaManager::IsAvailable() {
    return std::filesystem::exists(nebulaBinaryPath_) && 
           std::filesystem::exists(nebulaCertBinaryPath_);
}

std::string NebulaManager::GetVersion() {
    if (!IsAvailable()) {
        return "Not installed";
    }
    
    auto result = ExecuteNebula({"-version"}, false);
    if (result.success) {
        return result.errorMessage; // Version is returned in output
    }
    return "Unknown";
}

// ============================================================================
// Certificate Management
// ============================================================================

NebulaResult NebulaManager::GenerateCA(const std::string& networkName,
                                        const std::filesystem::path& outputDir,
                                        int validityDays) {
    NebulaResult result;
    
    if (!IsAvailable()) {
        result.errorMessage = "Nebula binaries not found";
        return result;
    }
    
    // Create output directory if needed
    std::error_code ec;
    std::filesystem::create_directories(outputDir, ec);
    if (ec) {
        result.errorMessage = "Failed to create output directory: " + ec.message();
        return result;
    }
    
    // Build command arguments
    std::vector<std::string> args = {
        "ca",
        "-name", networkName,
        "-duration", std::to_string(validityDays * 24) + "h",
        "-out-crt", (outputDir / "ca.crt").string(),
        "-out-key", (outputDir / "ca.key").string()
    };
    
    result = ExecuteNebulaCert(args, outputDir);
    
    if (result.success) {
        LOGF_INFO("[Nebula] Generated CA certificate for network: {}", networkName);
    }
    
    return result;
}

NebulaResult NebulaManager::GenerateNodeCert(const std::string& nodeName,
                                              const std::string& ipAddress,
                                              const std::filesystem::path& caDir,
                                              const std::filesystem::path& outputDir,
                                              bool isLighthouse) {
    NebulaResult result;
    
    if (!IsAvailable()) {
        result.errorMessage = "Nebula binaries not found";
        return result;
    }
    
    // Verify CA files exist
    auto caCertPath = caDir / "ca.crt";
    auto caKeyPath = caDir / "ca.key";
    
    if (!std::filesystem::exists(caCertPath) || !std::filesystem::exists(caKeyPath)) {
        result.errorMessage = "CA certificate or key not found in: " + caDir.string();
        return result;
    }
    
    // Create output directory if needed
    std::error_code ec;
    std::filesystem::create_directories(outputDir, ec);
    
    // Build command arguments
    std::vector<std::string> args = {
        "sign",
        "-name", nodeName,
        "-ip", ipAddress,
        "-ca-crt", caCertPath.string(),
        "-ca-key", caKeyPath.string(),
        "-out-crt", (outputDir / (nodeName + ".crt")).string(),
        "-out-key", (outputDir / (nodeName + ".key")).string()
    };
    
    // Add lighthouse group if applicable
    if (isLighthouse) {
        args.push_back("-groups");
        args.push_back("lighthouse");
    }
    
    result = ExecuteNebulaCert(args, outputDir);
    
    if (result.success) {
        LOGF_INFO("[Nebula] Generated certificate for node: {} ({})", nodeName, ipAddress);
    }
    
    return result;
}

NebulaResult NebulaManager::ImportCACert(const std::string& caCertData,
                                          const std::filesystem::path& outputDir) {
    NebulaResult result;
    
    // Create output directory if needed
    std::error_code ec;
    std::filesystem::create_directories(outputDir, ec);
    if (ec) {
        result.errorMessage = "Failed to create output directory: " + ec.message();
        return result;
    }
    
    // Write CA certificate to file
    auto caCertPath = outputDir / "ca.crt";
    std::ofstream outFile(caCertPath);
    if (!outFile.is_open()) {
        result.errorMessage = "Failed to create CA certificate file";
        return result;
    }
    
    outFile << caCertData;
    outFile.close();
    
    result.success = true;
    LOGF_INFO("[Nebula] Imported CA certificate to: {}", caCertPath.string());
    
    return result;
}

std::string NebulaManager::ExportCACert(const std::filesystem::path& caCertPath) {
    if (!std::filesystem::exists(caCertPath)) {
        return "";
    }
    
    std::ifstream inFile(caCertPath);
    if (!inFile.is_open()) {
        return "";
    }
    
    std::stringstream buffer;
    buffer << inFile.rdbuf();
    return buffer.str();
}

// ============================================================================
// Configuration
// ============================================================================

NebulaResult NebulaManager::CreateConfig(const NebulaConfig& config,
                                          const std::filesystem::path& outputPath) {
    NebulaResult result;
    
    // Load appropriate template
    std::string templateName = config.isLighthouse ? "lighthouse.yml.template" : "client.yml.template";
    std::string templateContent = LoadTemplate(templateName);
    
    if (templateContent.empty()) {
        result.errorMessage = "Failed to load configuration template: " + templateName;
        return result;
    }
    
    // Process template
    std::string configContent = ProcessTemplate(templateContent, config);
    
    // Create output directory if needed
    std::error_code ec;
    std::filesystem::create_directories(outputPath.parent_path(), ec);
    
    // Write configuration file
    std::ofstream outFile(outputPath);
    if (!outFile.is_open()) {
        result.errorMessage = "Failed to create configuration file: " + outputPath.string();
        return result;
    }
    
    outFile << configContent;
    outFile.close();
    
    result.success = true;
    LOGF_INFO("[Nebula] Created configuration file: {}", outputPath.string());
    
    return result;
}

bool NebulaManager::LoadConfig(const std::filesystem::path& configPath, NebulaConfig& config) {
    // Basic YAML parsing - just extract key values
    if (!std::filesystem::exists(configPath)) {
        return false;
    }
    
    std::ifstream inFile(configPath);
    if (!inFile.is_open()) {
        return false;
    }
    
    std::string line;
    while (std::getline(inFile, line)) {
        // Simple key-value extraction
        if (line.find("am_lighthouse: true") != std::string::npos) {
            config.isLighthouse = true;
        }
        // Add more parsing as needed
    }
    
    return true;
}

bool NebulaManager::HasValidConfig() {
    auto configPath = GetNebulaDirectory() / "config.yml";
    if (!std::filesystem::exists(configPath)) {
        return false;
    }
    
    // Check if certificates exist
    auto nebulaDir = GetNebulaDirectory();
    return std::filesystem::exists(nebulaDir / "ca.crt");
}

// ============================================================================
// Service Management
// ============================================================================

NebulaResult NebulaManager::Start(const std::filesystem::path& configPath) {
    NebulaResult result;
    
    if (!IsAvailable()) {
        result.errorMessage = "Nebula binaries not found";
        return result;
    }
    
    if (!std::filesystem::exists(configPath)) {
        result.errorMessage = "Configuration file not found: " + configPath.string();
        return result;
    }
    
    if (isRunning_) {
        result.success = true;
        result.errorMessage = "Nebula is already running";
        return result;
    }
    
#ifdef _WIN32
    // Windows: Use CreateProcess
    STARTUPINFOA si = {sizeof(si)};
    PROCESS_INFORMATION pi;
    
    std::string cmdLine = "\"" + nebulaBinaryPath_.string() + "\" -config \"" + configPath.string() + "\"";
    
    if (CreateProcessA(
        nullptr,
        cmdLine.data(),
        nullptr,
        nullptr,
        FALSE,
        CREATE_NO_WINDOW | DETACHED_PROCESS,
        nullptr,
        nullptr,
        &si,
        &pi
    )) {
        nebulaProcessId_ = pi.dwProcessId;
        nebulaProcessHandle_ = pi.hProcess;
        CloseHandle(pi.hThread);
        isRunning_ = true;
        result.success = true;
        LOGF_INFO("[Nebula] Started with PID: {}", nebulaProcessId_);
    } else {
        result.errorMessage = "Failed to start Nebula: " + std::to_string(GetLastError());
    }
#else
    // Unix: Use posix_spawn for background process
    pid_t pid;
    
    std::vector<char*> args;
    std::string binaryPath = nebulaBinaryPath_.string();
    std::string configArg = "-config";
    std::string configPathStr = configPath.string();
    
    args.push_back(const_cast<char*>(binaryPath.c_str()));
    args.push_back(const_cast<char*>(configArg.c_str()));
    args.push_back(const_cast<char*>(configPathStr.c_str()));
    args.push_back(nullptr);
    
    posix_spawnattr_t attr;
    posix_spawnattr_init(&attr);
    posix_spawnattr_setflags(&attr, POSIX_SPAWN_SETSIGDEF);
    
    int status = posix_spawn(&pid, binaryPath.c_str(), nullptr, &attr, args.data(), environ);
    posix_spawnattr_destroy(&attr);
    
    if (status == 0) {
        nebulaProcessId_ = pid;
        isRunning_ = true;
        result.success = true;
        LOGF_INFO("[Nebula] Started with PID: {}", nebulaProcessId_);
    } else {
        result.errorMessage = "Failed to start Nebula: " + std::string(strerror(status));
    }
#endif

    return result;
}

NebulaResult NebulaManager::Stop() {
    NebulaResult result;
    
    if (!isRunning_) {
        result.success = true;
        return result;
    }
    
#ifdef _WIN32
    if (nebulaProcessHandle_) {
        TerminateProcess(nebulaProcessHandle_, 0);
        CloseHandle(nebulaProcessHandle_);
        nebulaProcessHandle_ = nullptr;
    }
#else
    if (nebulaProcessId_ > 0) {
        kill(nebulaProcessId_, SIGTERM);
        
        // Wait for graceful shutdown
        int status;
        waitpid(nebulaProcessId_, &status, WNOHANG);
    }
#endif

    isRunning_ = false;
    nebulaProcessId_ = -1;
    result.success = true;
    
    LOG("[Nebula] Stopped");
    
    return result;
}

bool NebulaManager::IsRunning() {
    if (!isRunning_) {
        return false;
    }
    
#ifdef _WIN32
    if (nebulaProcessHandle_) {
        DWORD exitCode;
        if (GetExitCodeProcess(nebulaProcessHandle_, &exitCode)) {
            if (exitCode != STILL_ACTIVE) {
                isRunning_ = false;
                CloseHandle(nebulaProcessHandle_);
                nebulaProcessHandle_ = nullptr;
            }
        }
    }
#else
    if (nebulaProcessId_ > 0) {
        int status;
        pid_t result = waitpid(nebulaProcessId_, &status, WNOHANG);
        if (result != 0) {
            isRunning_ = false;
        }
    }
#endif

    return isRunning_;
}

NebulaStatus NebulaManager::GetStatus() {
    if (!IsAvailable()) {
        return NebulaStatus::NotInstalled;
    }
    
    if (!HasValidConfig()) {
        return NebulaStatus::NotConfigured;
    }
    
    if (IsRunning()) {
        return NebulaStatus::Running;
    }
    
    return NebulaStatus::Stopped;
}

std::vector<std::string> NebulaManager::GetConnectedPeers() {
    // TODO: Parse nebula status output or query API
    std::vector<std::string> peers;
    return peers;
}

// ============================================================================
// Utility
// ============================================================================

NebulaResult NebulaManager::TestConnection(const std::string& lighthouseAddress) {
    NebulaResult result;
    
    // Simple connectivity test - try to reach the lighthouse port
    // This is a basic implementation; could be enhanced with actual ICMP/UDP checks
    
    result.success = true;
    result.errorMessage = "Connection test not fully implemented";
    
    return result;
}

std::string NebulaManager::GetPublicIP() {
    // Try to get public IP from external service
    // This is a placeholder - actual implementation would use HTTP request
    return "";
}

std::string NebulaManager::GenerateNetworkName() {
    // Generate a random network name
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1000, 9999);
    
    return "LibertyRecomp-" + std::to_string(dis(gen));
}

std::string NebulaManager::SuggestNextIP(const std::string& networkRange,
                                          const std::vector<std::string>& usedIPs) {
    // Simple implementation - find first unused IP in 192.168.100.x range
    for (int i = 2; i <= 254; i++) {
        std::string ip = "192.168.100." + std::to_string(i);
        bool used = false;
        for (const auto& usedIP : usedIPs) {
            if (usedIP.find(ip) != std::string::npos) {
                used = true;
                break;
            }
        }
        if (!used) {
            return ip + "/24";
        }
    }
    return "192.168.100.2/24";
}

// ============================================================================
// Private Methods
// ============================================================================

NebulaResult NebulaManager::ExecuteNebulaCert(const std::vector<std::string>& args,
                                               const std::filesystem::path& workDir) {
    NebulaResult result;
    
    std::string cmdLine = nebulaCertBinaryPath_.string();
    for (const auto& arg : args) {
        cmdLine += " ";
        if (arg.find(' ') != std::string::npos) {
            cmdLine += "\"" + arg + "\"";
        } else {
            cmdLine += arg;
        }
    }
    
    LOGF_DEBUG("[Nebula] Executing: {}", cmdLine);
    
#ifdef _WIN32
    STARTUPINFOA si = {sizeof(si)};
    PROCESS_INFORMATION pi;
    
    si.dwFlags = STARTF_USESHOWWINDOW;
    si.wShowWindow = SW_HIDE;
    
    if (CreateProcessA(
        nullptr,
        cmdLine.data(),
        nullptr,
        nullptr,
        FALSE,
        CREATE_NO_WINDOW,
        nullptr,
        workDir.string().c_str(),
        &si,
        &pi
    )) {
        WaitForSingleObject(pi.hProcess, INFINITE);
        
        DWORD exitCode;
        GetExitCodeProcess(pi.hProcess, &exitCode);
        result.exitCode = static_cast<int>(exitCode);
        result.success = (exitCode == 0);
        
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
    } else {
        result.errorMessage = "Failed to execute nebula-cert: " + std::to_string(GetLastError());
    }
#else
    // Save current directory and change to workDir
    auto originalDir = std::filesystem::current_path();
    std::filesystem::current_path(workDir);
    
    result.exitCode = std::system(cmdLine.c_str());
    result.success = (result.exitCode == 0);
    
    // Restore directory
    std::filesystem::current_path(originalDir);
    
    if (!result.success) {
        result.errorMessage = "nebula-cert exited with code: " + std::to_string(result.exitCode);
    }
#endif

    return result;
}

NebulaResult NebulaManager::ExecuteNebula(const std::vector<std::string>& args,
                                           bool background) {
    NebulaResult result;
    
    std::string cmdLine = nebulaBinaryPath_.string();
    for (const auto& arg : args) {
        cmdLine += " " + arg;
    }
    
    if (background) {
        // Use Start() for background execution
        result.errorMessage = "Use Start() for background execution";
        return result;
    }
    
    // Blocking execution for version check, etc.
    result.exitCode = std::system(cmdLine.c_str());
    result.success = (result.exitCode == 0);
    
    return result;
}

std::string NebulaManager::LoadTemplate(const std::string& templateName) {
    auto templatePath = configTemplateDir_ / templateName;
    
    // Try install directory first
    if (!std::filesystem::exists(templatePath)) {
        // Fall back to source directory (for development)
        templatePath = std::filesystem::path(__FILE__).parent_path().parent_path().parent_path() 
                       / "tools" / "nebula" / "config" / templateName;
    }
    
    if (!std::filesystem::exists(templatePath)) {
        LOGF_ERROR("[Nebula] Template not found: {}", templateName);
        return "";
    }
    
    std::ifstream inFile(templatePath);
    if (!inFile.is_open()) {
        return "";
    }
    
    std::stringstream buffer;
    buffer << inFile.rdbuf();
    return buffer.str();
}

std::string NebulaManager::ProcessTemplate(const std::string& templateContent,
                                            const NebulaConfig& config) {
    std::string result = templateContent;
    
    auto replaceAll = [&result](const std::string& from, const std::string& to) {
        size_t pos = 0;
        while ((pos = result.find(from, pos)) != std::string::npos) {
            result.replace(pos, from.length(), to);
            pos += to.length();
        }
    };
    
    // Replace template variables
    replaceAll("{{CA_CERT_PATH}}", config.caCertPath.string());
    replaceAll("{{HOST_CERT_PATH}}", config.nodeCertPath.string());
    replaceAll("{{HOST_KEY_PATH}}", config.nodeKeyPath.string());
    replaceAll("{{LIGHTHOUSE_IP}}", config.lighthouseVirtualIp);
    replaceAll("{{PUBLIC_IP}}", config.lighthousePublicAddress);
    replaceAll("{{LIGHTHOUSE_PUBLIC}}", config.lighthousePublicAddress + ":" + std::to_string(config.listenPort));
    replaceAll("{{LISTEN_PORT}}", std::to_string(config.listenPort));
    
    return result;
}

} // namespace liberty::install
