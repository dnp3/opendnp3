#ifndef EXAMPLE_LISTENCALLBACKS_H
#define EXAMPLE_LISTENCALLBACKS_H

#include <asiodnp3/IListenCallbacks.h>
#include <openpal/logging/Logger.h>

#include <mutex>
#include <vector>

class ExampleListenCallbacks final : public asiodnp3::IListenCallbacks
{
    struct SessionInfo
    {
        uint64_t id;
        uint16_t address;
        std::shared_ptr<asiodnp3::IMasterSession> session;
    };

public:

    // ------ Retrieve a (possibly nullptr) master session reference to control a particular outstation --------

    std::shared_ptr<asiodnp3::IMasterSession> get_outstation_session(uint16_t address);

    // ------ Implement IListenCallbacks --------

	bool AcceptConnection(uint64_t sessionid, const std::string& ipaddress) override;
	
	bool AcceptCertificate(uint64_t sessionid, const asiodnp3::X509Info& info) override;

	openpal::TimeDuration GetFirstFrameTimeout() override;
	
	void OnFirstFrame(uint64_t sessionid, const opendnp3::LinkHeaderFields& header, asiodnp3::ISessionAcceptor& acceptor) override;

	void OnConnectionClose(uint64_t sessionid, const std::shared_ptr<asiodnp3::IMasterSession>& session) override;

	void OnCertificateError(uint64_t sessionid, const asiodnp3::X509Info& info, int error) override;

private:

	static std::string GetSessionName(uint16_t outstation_address, uint64_t session_id);

	// synchronize access to all class methods since callbacks can come from library or userland
	std::mutex mutex;

	// more sophisticated implementations might use a map for faster lookup
	std::vector<SessionInfo> sessions;
};



#endif

