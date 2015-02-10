/*  Copyright 2015 MaidSafe.net limited

    This MaidSafe Software is licensed to you under (1) the MaidSafe.net Commercial License,
    version 1.0 or later, or (2) The General Public License (GPL), version 3, depending on which
    licence you accepted on initial access to the Software (the "Licences").

    By contributing code to the MaidSafe Software, or to this project generally, you agree to be
    bound by the terms of the MaidSafe Contributor Agreement, version 1.0, found in the root
    directory of this project at LICENSE, COPYING and CONTRIBUTOR respectively and also
    available at: http://www.maidsafe.net/licenses

    Unless required by applicable law or agreed to in writing, the MaidSafe Software distributed
    under the GPL Licence is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS
    OF ANY KIND, either express or implied.

    See the Licences for the specific language governing permissions and limitations relating to
    use of the MaidSafe Software.                                                                 */

#ifndef MAIDSAFE_ROUTING_CLIENT_H_
#define MAIDSAFE_ROUTING_CLIENT_H_

#include <atomic>
#include <cstdint>
#include <memory>
#include <utility>

#include "asio/io_service.hpp"
#include "boost/filesystem/path.hpp"
#include "boost/optional/optional.hpp"

#include "maidsafe/common/node_id.h"
#include "maidsafe/common/rsa.h"
#include "maidsafe/common/types.h"
#include "maidsafe/common/containers/lru_cache.h"

#include "maidsafe/routing/bootstrap_handler.h"
#include "maidsafe/routing/sentinel.h"
#include "maidsafe/routing/types.h"
#include "maidsafe/routing/messages/messages_fwd.h"

namespace maidsafe {

namespace routing {

class Client : public std::enable_shared_from_this<Client> {
 public:
  Client(asio::io_service& io_service, boost::filesystem::path db_location, Identity our_id,
         asymm::Keys our_keys);
  Client() = delete;
  Client(const Client&) = delete;
  Client(Client&&) = delete;
  Client& operator=(const Client&) = delete;
  Client& operator=(Client&&) = delete;
  ~Client();

  // normal bootstrap mechanism
  template <typename CompletionToken>
  BootstrapReturn<CompletionToken> Bootstrap(CompletionToken token);
  // used where we wish to pass a specific node to bootstrap from
  template <typename CompletionToken>
  BootstrapReturn<CompletionToken> Bootstrap(Endpoint endpoint, CompletionToken token);
  // will return with the data
  template <typename CompletionToken>
  GetReturn<CompletionToken> Get(Address data_key, CompletionToken token);
  // will return with allowed or not (error_code only)
  template <typename CompletionToken>
  PutReturn<CompletionToken> Put(Address key, SerialisedMessage message, CompletionToken token);
  // TODO(dirvine) Weird this hides the POst type below in HandlePost)  :06/01/2015
  // will return with allowed or not (error_code only)
  template <typename CompletionToken>
  PostReturn<CompletionToken> Post(Address key, SerialisedMessage message, CompletionToken token);
  // will return with response message
  template <typename CompletionToken>
  RequestReturn<CompletionToken> Request(Address key, SerialisedMessage message,
                                         CompletionToken token);
  Address OurId() const { return our_id_; }

 private:
  virtual void MessageReceived(NodeId peer_id, std::vector<unsigned char> message);
  virtual void ConnectionLost(NodeId peer);

  void OnCloseGroupChanged(CloseGroupDifference close_group_difference);
  void HandleMessage(ConnectResponse&& connect_response);
  void HandleMessage(GetDataResponse&& get_data_response);
  void HandleMessage(PostMessage&& post_message);
  void HandleMessage(PostResponse&& post_response);

  asio::io_service& io_service_;
  Address our_id_;
  asymm::Keys our_keys_;
  boost::optional<Address> bootstrap_node_;
  std::atomic<MessageId> message_id_;
  //rudp::ManagedConnections rudp_;
  BootstrapHandler bootstrap_handler_;
  LruCache<std::pair<Address, MessageId>, void> filter_;
  Sentinel sentinel_;
};

template <typename CompletionToken>
BootstrapReturn<CompletionToken> Client::Bootstrap(CompletionToken token) {
  auto handler(std::forward<decltype(token)>(token));
  auto result(handler);
  auto this_ptr(shared_from_this());
  io_service_.post([=] {
// TODO(PeterJ)
//    rudp_.Bootstrap(bootstrap_handler_.ReadBootstrapContacts(), this_ptr, our_id_, our_keys_,
//                    handler);
  });
  return result.get();
}

template <typename CompletionToken>
BootstrapReturn<CompletionToken> Client::Bootstrap(Endpoint local_endpoint, CompletionToken token) {
  auto handler(std::forward<decltype(token)>(token));
  auto result(handler);
  auto this_ptr(shared_from_this());
  io_service_.post([=] {
// TODO(PeterJ)
//    rudp_.Bootstrap(bootstrap_handler_.ReadBootstrapContacts(), this_ptr, our_id_, our_keys_,
//                    handler, local_endpoint);
  });
  return result.get();
}

template <typename CompletionToken>
GetReturn<CompletionToken> Client::Get(Address /*data_key*/, CompletionToken token) {
  auto handler(std::forward<decltype(token)>(token));
  auto result(handler);
  auto this_ptr(shared_from_this());
//  io_service_.post([=] { this_ptr->DoGet(data_key, handler); });
  return result.get();
}

template <typename CompletionToken>
PutReturn<CompletionToken> Client::Put(Address /*key*/, SerialisedMessage /*message*/,
                                       CompletionToken token) {
  auto handler(std::forward<decltype(token)>(token));
  auto result(handler);
  auto this_ptr(shared_from_this());
//  io_service_.post([=] { this_ptr->DoPut(key, message, handler); });
  return result.get();
}

template <typename CompletionToken>
PostReturn<CompletionToken> Client::Post(Address /*key*/, SerialisedMessage /*message*/,
                                         CompletionToken token) {
  auto handler(std::forward<decltype(token)>(token));
  auto result(handler);
  auto this_ptr(shared_from_this());
//  io_service_.post([=] { this_ptr->DoPost(key, message, handler); });
  return result.get();
}

}  // namespace routing

}  // namespace maidsafe

#endif  // MAIDSAFE_ROUTING_CLIENT_H_
