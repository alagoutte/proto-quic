// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "net/quic/chromium/mock_crypto_client_stream_factory.h"

#include "base/lazy_instance.h"
#include "net/quic/chromium/quic_chromium_client_session.h"
#include "net/quic/core/quic_crypto_client_stream.h"

using std::string;

namespace net {

MockCryptoClientStreamFactory::~MockCryptoClientStreamFactory() {}

MockCryptoClientStreamFactory::MockCryptoClientStreamFactory()
    : handshake_mode_(MockCryptoClientStream::CONFIRM_HANDSHAKE),
      last_stream_(nullptr),
      config_(new QuicConfig()) {}

void MockCryptoClientStreamFactory::SetConfig(const QuicConfig& config) {
  config_.reset(new QuicConfig(config));
}

QuicCryptoClientStream*
MockCryptoClientStreamFactory::CreateQuicCryptoClientStream(
    const QuicServerId& server_id,
    QuicChromiumClientSession* session,
    std::unique_ptr<ProofVerifyContext> /*proof_verify_context*/,
    QuicCryptoClientConfig* crypto_config) {
  const ProofVerifyDetailsChromium* proof_verify_details = nullptr;
  if (!proof_verify_details_queue_.empty()) {
    proof_verify_details = proof_verify_details_queue_.front();
    proof_verify_details_queue_.pop();
  }
  last_stream_ = new MockCryptoClientStream(
      server_id, session, nullptr, *(config_.get()), crypto_config,
      handshake_mode_, proof_verify_details);
  return last_stream_;
}

}  // namespace net
