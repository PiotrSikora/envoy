#pragma once

#include "envoy/api/api.h"
#include "envoy/http/codec.h"
#include "envoy/http/header_map.h"
#include "envoy/network/filter.h"

#include "common/http/codec_client.h"

/**
 * A buffering response decoder used for testing.
 */
class BufferingStreamDecoder : public Http::StreamDecoder, public Http::StreamCallbacks {
public:
  BufferingStreamDecoder(std::function<void()> on_complete_cb) : on_complete_cb_(on_complete_cb) {}

  bool complete() { return complete_; }
  const Http::HeaderMap& headers() { return *headers_; }
  const std::string& body() { return body_; }

  // Http::StreamDecoder
  void decodeHeaders(Http::HeaderMapPtr&& headers, bool end_stream) override;
  void decodeData(const Buffer::Instance&, bool end_stream) override;
  void decodeTrailers(Http::HeaderMapPtr&& trailers) override;

  // Http::StreamCallbacks
  void onResetStream(Http::StreamResetReason reason) override;

private:
  void onComplete();

  Http::HeaderMapPtr headers_;
  std::string body_;
  bool complete_{};
  std::function<void()> on_complete_cb_;
};

typedef std::unique_ptr<BufferingStreamDecoder> BufferingStreamDecoderPtr;

/**
 * Basic driver for a raw connection.
 */
class RawConnectionDriver {
public:
  typedef std::function<void(Network::ClientConnection&, const Buffer::Instance&)> ReadCallback;

  RawConnectionDriver(uint32_t port, Buffer::Instance& initial_data, ReadCallback data_callback);
  ~RawConnectionDriver();
  void run();
  void close();

private:
  struct ForwardingFilter : public Network::ReadFilterBaseImpl {
    ForwardingFilter(RawConnectionDriver& parent, ReadCallback cb)
        : parent_(parent), data_callback_(cb) {}

    // Network::ReadFilter
    Network::FilterStatus onData(Buffer::Instance& data) override {
      data_callback_(*parent_.client_, data);
      return Network::FilterStatus::StopIteration;
    }

    RawConnectionDriver& parent_;
    ReadCallback data_callback_;
  };

  Api::ApiPtr api_;
  Event::DispatcherPtr dispatcher_;
  Network::ClientConnectionPtr client_;
};

/**
 * Utility routines for integration tests.
 */
class IntegrationUtil {
public:
  /**
   * Make a new connection, issues a request, and then disconnect when the request is complete.
   * @param port supplies the port to connect to on localhost.
   * @param method supplies the request method.
   * @param url supplies the request url.
   * @param body supplies the optional request body to send.
   * @param type supplies the codec to use for the request.
   * @param host supplies the host header to use for the request.
   * @return BufferingStreamDecoderPtr the complete request or a partial request if there was
   *         remote easly disconnection.
   */
  static BufferingStreamDecoderPtr makeSingleRequest(uint32_t port, const std::string& method,
                                                     const std::string& url,
                                                     const std::string& body,
                                                     Http::CodecClient::Type type,
                                                     const std::string& host = "host");
};
