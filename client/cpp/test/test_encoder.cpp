#include <gtest/gtest.h>

#include <string>

#include <krpc/client.hpp>
#include <krpc/encoder.hpp>
#include <krpc/platform.hpp>

#include "services/test_service.hpp"

TEST(test_encoder, test_rpc_hello_message) {
  std::string message(krpc::encoder::RPC_HELLO_MESSAGE, krpc::encoder::RPC_HELLO_MESSAGE_LENGTH);
  ASSERT_EQ(8, message.size());
  ASSERT_EQ("4b5250432d525043", krpc::platform::hexlify(message));
}

TEST(test_encoder, test_stream_hello_message) {
  std::string message(krpc::encoder::STREAM_HELLO_MESSAGE, krpc::encoder::STREAM_HELLO_MESSAGE_LENGTH);
  ASSERT_EQ(8, message.size());
  ASSERT_EQ("4b5250432d535452", krpc::platform::hexlify(message));
}

TEST(test_encoder, test_encode_message) {
  krpc::schema::Request request;
  request.set_service("ServiceName");
  request.set_procedure("ProcedureName");
  std::string data = krpc::encoder::encode(request);
  std::string expected = "0a0b536572766963654e616d65120d50726f6365647572654e616d65";
  ASSERT_EQ(expected, krpc::platform::hexlify(data));
}

TEST(test_encoder, test_encode_value) {
  std::string data = krpc::encoder::encode((unsigned int)300);
  ASSERT_EQ("ac02", krpc::platform::hexlify(data));
}

TEST(test_encoder, test_encode_string) {
  std::string data = krpc::encoder::encode("foo");
  ASSERT_EQ("03666f6f", krpc::platform::hexlify(data));
}

TEST(test_encoder, test_encode_unicode_string) {
  krpc::platform::unhexlify("6a");
  std::string in = krpc::platform::unhexlify("e284a2");
  std::string data = krpc::encoder::encode(in);
  ASSERT_EQ("03e284a2", krpc::platform::hexlify(data));
}

TEST(test_encoder, test_encode_message_with_size) {
  krpc::schema::Request request;
  request.set_service("ServiceName");
  request.set_procedure("ProcedureName");
  std::string data = krpc::encoder::encode_message_with_size(request);
  std::string expected = "1c0a0b536572766963654e616d65120d50726f6365647572654e616d65";
  ASSERT_EQ(expected, krpc::platform::hexlify(data));
}

TEST(test_encoder, test_encode_class) {
  krpc::services::TestService::TestClass value(nullptr, 300);
  std::string data = krpc::encoder::encode(value);
  ASSERT_EQ("ac02", krpc::platform::hexlify(data));
}

TEST(test_encoder, test_encode_class_none) {
  krpc::services::TestService::TestClass value;
  std::string data = krpc::encoder::encode(value);
  ASSERT_EQ("00", krpc::platform::hexlify(data));
}
