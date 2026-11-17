#ifndef AUTH_MANAGER_HPP
#define AUTH_MANAGER_HPP

#include <td/telegram/td_api.h>
#include <functional>
#include <atomic>
#include <memory>

namespace td_api = td::td_api;

class TdClient; // Forward declaration

class AuthManager {
public:
    using AuthCallback = std::function<void(bool success)>;

    AuthManager(TdClient& client);

    // Starts the auth process. Returns true if already authorized.
    bool check_auth();

    // Sets a callback for when authorization is complete.
    void set_on_auth_complete(AuthCallback callback);

    // Handles incoming updates related to authorization state.
    void handle_update(td_api::object_ptr<td_api::Object>&& update);

private:
    TdClient& client_;
    AuthCallback on_auth_complete_;
    std::atomic<bool> is_authorized_{false};
    
    void process_authorization_state(td_api::object_ptr<td_api::AuthorizationState> state);
    void handle_wait_params();
    void handle_wait_qr(td_api::authorizationStateWaitOtherDeviceConfirmation& s);
};

#endif // AUTH_MANAGER_HPP
