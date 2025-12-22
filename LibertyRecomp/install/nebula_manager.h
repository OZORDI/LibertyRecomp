#pragma once

#include <filesystem>
#include <string>
#include <vector>
#include <functional>
#include <atomic>

namespace liberty::install {

/**
 * Nebula VPN Setup Mode
 */
enum class NebulaSetupMode {
    Skip,           // User opts out of online multiplayer
    JoinExisting,   // Join existing network (requires network ID + CA cert)
    CreateNew       // Create new network (becomes lighthouse)
};

/**
 * Nebula Network Configuration
 */
struct NebulaConfig {
    NebulaSetupMode mode = NebulaSetupMode::Skip;
    std::string networkName;
    std::string nodeName;           // This node's name (e.g., "player1")
    std::string ipAddress;          // Virtual IP (e.g., "192.168.100.2/24")
    bool isLighthouse = false;
    std::string lighthouseVirtualIp; // Lighthouse virtual IP (e.g., "192.168.100.1")
    std::string lighthousePublicAddress; // Public IP:port of lighthouse
    uint16_t listenPort = 4242;     // UDP port for lighthouse
    
    // Certificate paths (set after generation)
    std::filesystem::path caCertPath;
    std::filesystem::path caKeyPath;
    std::filesystem::path nodeCertPath;
    std::filesystem::path nodeKeyPath;
};

/**
 * Result of Nebula operations
 */
struct NebulaResult {
    bool success = false;
    std::string errorMessage;
    int exitCode = 0;
};

/**
 * Nebula connection status
 */
enum class NebulaStatus {
    NotInstalled,   // Nebula binaries not found
    NotConfigured,  // No configuration file
    Stopped,        // Configured but not running
    Starting,       // Process starting
    Running,        // Connected and running
    Error           // Error state
};

/**
 * NebulaManager - Handles Nebula VPN integration for online multiplayer
 * 
 * Responsibilities:
 * - Locate and manage Nebula binaries
 * - Generate CA and node certificates
 * - Create configuration files from templates
 * - Start/stop Nebula service
 * - Monitor connection status
 */
class NebulaManager {
public:
    static NebulaManager& Instance();
    
    // =========================================================================
    // Initialization
    // =========================================================================
    
    /**
     * Check if Nebula binaries are available
     */
    bool IsAvailable();
    
    /**
     * Get Nebula binary version
     */
    std::string GetVersion();
    
    /**
     * Get platform-specific Nebula directory
     * Windows: %LOCALAPPDATA%\LibertyRecomp\nebula\
     * macOS:   ~/Library/Application Support/LibertyRecomp/nebula/
     * Linux:   ~/.local/share/LibertyRecomp/nebula/
     */
    static std::filesystem::path GetNebulaDirectory();
    
    /**
     * Get path to Nebula binary
     */
    std::filesystem::path GetNebulaBinaryPath();
    
    /**
     * Get path to nebula-cert binary
     */
    std::filesystem::path GetNebulaCertBinaryPath();
    
    // =========================================================================
    // Certificate Management
    // =========================================================================
    
    /**
     * Generate CA certificate for a new network
     * Creates ca.crt and ca.key in the specified output directory
     * 
     * @param networkName Name for the CA (e.g., "LibertyRecomp-MyNetwork")
     * @param outputDir Directory to write certificates
     * @param validityDays Certificate validity in days (default: 365)
     */
    NebulaResult GenerateCA(const std::string& networkName,
                            const std::filesystem::path& outputDir,
                            int validityDays = 365);
    
    /**
     * Generate node certificate signed by CA
     * Creates <nodeName>.crt and <nodeName>.key in the output directory
     * 
     * @param nodeName Name for this node (e.g., "lighthouse", "player1")
     * @param ipAddress Virtual IP with CIDR (e.g., "192.168.100.1/24")
     * @param caDir Directory containing ca.crt and ca.key
     * @param outputDir Directory to write node certificate
     * @param isLighthouse Whether this node is a lighthouse
     */
    NebulaResult GenerateNodeCert(const std::string& nodeName,
                                  const std::string& ipAddress,
                                  const std::filesystem::path& caDir,
                                  const std::filesystem::path& outputDir,
                                  bool isLighthouse = false);
    
    /**
     * Import CA certificate from another network
     * 
     * @param caCertData Base64-encoded CA certificate
     * @param outputDir Directory to write ca.crt
     */
    NebulaResult ImportCACert(const std::string& caCertData,
                              const std::filesystem::path& outputDir);
    
    /**
     * Export CA certificate for sharing
     * 
     * @param caCertPath Path to ca.crt file
     * @return Base64-encoded CA certificate
     */
    std::string ExportCACert(const std::filesystem::path& caCertPath);
    
    // =========================================================================
    // Configuration
    // =========================================================================
    
    /**
     * Create Nebula configuration file from template
     * 
     * @param config Configuration parameters
     * @param outputPath Path to write config.yml
     */
    NebulaResult CreateConfig(const NebulaConfig& config,
                              const std::filesystem::path& outputPath);
    
    /**
     * Load existing configuration
     */
    bool LoadConfig(const std::filesystem::path& configPath, NebulaConfig& config);
    
    /**
     * Check if a valid configuration exists
     */
    bool HasValidConfig();
    
    // =========================================================================
    // Service Management
    // =========================================================================
    
    /**
     * Start Nebula service
     * 
     * @param configPath Path to config.yml
     */
    NebulaResult Start(const std::filesystem::path& configPath);
    
    /**
     * Stop Nebula service
     */
    NebulaResult Stop();
    
    /**
     * Check if Nebula is currently running
     */
    bool IsRunning();
    
    /**
     * Get current connection status
     */
    NebulaStatus GetStatus();
    
    /**
     * Get list of connected peers
     */
    std::vector<std::string> GetConnectedPeers();
    
    // =========================================================================
    // Utility
    // =========================================================================
    
    /**
     * Test connectivity to lighthouse
     */
    NebulaResult TestConnection(const std::string& lighthouseAddress);
    
    /**
     * Get public IP address (for lighthouse setup)
     */
    std::string GetPublicIP();
    
    /**
     * Generate a unique network name
     */
    static std::string GenerateNetworkName();
    
    /**
     * Suggest next available IP in range
     */
    static std::string SuggestNextIP(const std::string& networkRange,
                                     const std::vector<std::string>& usedIPs);

private:
    NebulaManager();
    ~NebulaManager();
    
    // Prevent copying
    NebulaManager(const NebulaManager&) = delete;
    NebulaManager& operator=(const NebulaManager&) = delete;
    
    // Execute nebula-cert command
    NebulaResult ExecuteNebulaCert(const std::vector<std::string>& args,
                                   const std::filesystem::path& workDir);
    
    // Execute nebula command
    NebulaResult ExecuteNebula(const std::vector<std::string>& args,
                               bool background = false);
    
    // Load configuration template
    std::string LoadTemplate(const std::string& templateName);
    
    // Replace template variables
    std::string ProcessTemplate(const std::string& templateContent,
                                const NebulaConfig& config);
    
    // Member variables
    std::filesystem::path nebulaBinaryPath_;
    std::filesystem::path nebulaCertBinaryPath_;
    std::filesystem::path configTemplateDir_;
    
    std::atomic<bool> isRunning_{false};
    int nebulaProcessId_ = -1;
    
#ifdef _WIN32
    void* nebulaProcessHandle_ = nullptr;
#endif
};

} // namespace liberty::install
