// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: protomessage.proto

#define INTERNAL_SUPPRESS_PROTOBUF_FIELD_DEPRECATION
#include "protomessage.pb.h"

#include <algorithm>

#include <google/protobuf/stubs/common.h>
#include <google/protobuf/stubs/once.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/wire_format_lite_inl.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/reflection_ops.h>
#include <google/protobuf/wire_format.h>
// @@protoc_insertion_point(includes)

namespace {

const ::google::protobuf::Descriptor* Message_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  Message_reflection_ = NULL;

}  // namespace


void protobuf_AssignDesc_protomessage_2eproto() {
  protobuf_AddDesc_protomessage_2eproto();
  const ::google::protobuf::FileDescriptor* file =
    ::google::protobuf::DescriptorPool::generated_pool()->FindFileByName(
      "protomessage.proto");
  GOOGLE_CHECK(file != NULL);
  Message_descriptor_ = file->message_type(0);
  static const int Message_offsets_[6] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Message, type_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Message, username_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Message, salaname_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Message, ip_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Message, message_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Message, port_),
  };
  Message_reflection_ =
    new ::google::protobuf::internal::GeneratedMessageReflection(
      Message_descriptor_,
      Message::default_instance_,
      Message_offsets_,
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Message, _has_bits_[0]),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Message, _unknown_fields_),
      -1,
      ::google::protobuf::DescriptorPool::generated_pool(),
      ::google::protobuf::MessageFactory::generated_factory(),
      sizeof(Message));
}

namespace {

GOOGLE_PROTOBUF_DECLARE_ONCE(protobuf_AssignDescriptors_once_);
inline void protobuf_AssignDescriptorsOnce() {
  ::google::protobuf::GoogleOnceInit(&protobuf_AssignDescriptors_once_,
                 &protobuf_AssignDesc_protomessage_2eproto);
}

void protobuf_RegisterTypes(const ::std::string&) {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
    Message_descriptor_, &Message::default_instance());
}

}  // namespace

void protobuf_ShutdownFile_protomessage_2eproto() {
  delete Message::default_instance_;
  delete Message_reflection_;
}

void protobuf_AddDesc_protomessage_2eproto() {
  static bool already_here = false;
  if (already_here) return;
  already_here = true;
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  ::google::protobuf::DescriptorPool::InternalAddGeneratedFile(
    "\n\022protomessage.proto\"f\n\007Message\022\014\n\004type\030"
    "\001 \002(\r\022\020\n\010username\030\002 \002(\t\022\020\n\010salaname\030\003 \001("
    "\t\022\n\n\002ip\030\004 \002(\t\022\017\n\007message\030\005 \001(\t\022\014\n\004port\030\006"
    " \002(\r", 124);
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedFile(
    "protomessage.proto", &protobuf_RegisterTypes);
  Message::default_instance_ = new Message();
  Message::default_instance_->InitAsDefaultInstance();
  ::google::protobuf::internal::OnShutdown(&protobuf_ShutdownFile_protomessage_2eproto);
}

// Force AddDescriptors() to be called at static initialization time.
struct StaticDescriptorInitializer_protomessage_2eproto {
  StaticDescriptorInitializer_protomessage_2eproto() {
    protobuf_AddDesc_protomessage_2eproto();
  }
} static_descriptor_initializer_protomessage_2eproto_;

// ===================================================================

#ifndef _MSC_VER
const int Message::kTypeFieldNumber;
const int Message::kUsernameFieldNumber;
const int Message::kSalanameFieldNumber;
const int Message::kIpFieldNumber;
const int Message::kMessageFieldNumber;
const int Message::kPortFieldNumber;
#endif  // !_MSC_VER

Message::Message()
  : ::google::protobuf::Message() {
  SharedCtor();
  // @@protoc_insertion_point(constructor:Message)
}

void Message::InitAsDefaultInstance() {
}

Message::Message(const Message& from)
  : ::google::protobuf::Message() {
  SharedCtor();
  MergeFrom(from);
  // @@protoc_insertion_point(copy_constructor:Message)
}

void Message::SharedCtor() {
  ::google::protobuf::internal::GetEmptyString();
  _cached_size_ = 0;
  type_ = 0u;
  username_ = const_cast< ::std::string*>(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  salaname_ = const_cast< ::std::string*>(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  ip_ = const_cast< ::std::string*>(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  message_ = const_cast< ::std::string*>(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  port_ = 0u;
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

Message::~Message() {
  // @@protoc_insertion_point(destructor:Message)
  SharedDtor();
}

void Message::SharedDtor() {
  if (username_ != &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    delete username_;
  }
  if (salaname_ != &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    delete salaname_;
  }
  if (ip_ != &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    delete ip_;
  }
  if (message_ != &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    delete message_;
  }
  if (this != default_instance_) {
  }
}

void Message::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* Message::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return Message_descriptor_;
}

const Message& Message::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_protomessage_2eproto();
  return *default_instance_;
}

Message* Message::default_instance_ = NULL;

Message* Message::New() const {
  return new Message;
}

void Message::Clear() {
#define OFFSET_OF_FIELD_(f) (reinterpret_cast<char*>(      \
  &reinterpret_cast<Message*>(16)->f) - \
   reinterpret_cast<char*>(16))

#define ZR_(first, last) do {                              \
    size_t f = OFFSET_OF_FIELD_(first);                    \
    size_t n = OFFSET_OF_FIELD_(last) - f + sizeof(last);  \
    ::memset(&first, 0, n);                                \
  } while (0)

  if (_has_bits_[0 / 32] & 63) {
    ZR_(type_, port_);
    if (has_username()) {
      if (username_ != &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
        username_->clear();
      }
    }
    if (has_salaname()) {
      if (salaname_ != &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
        salaname_->clear();
      }
    }
    if (has_ip()) {
      if (ip_ != &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
        ip_->clear();
      }
    }
    if (has_message()) {
      if (message_ != &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
        message_->clear();
      }
    }
  }

#undef OFFSET_OF_FIELD_
#undef ZR_

  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  mutable_unknown_fields()->Clear();
}

bool Message::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) goto failure
  ::google::protobuf::uint32 tag;
  // @@protoc_insertion_point(parse_start:Message)
  for (;;) {
    ::std::pair< ::google::protobuf::uint32, bool> p = input->ReadTagWithCutoff(127);
    tag = p.first;
    if (!p.second) goto handle_unusual;
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // required uint32 type = 1;
      case 1: {
        if (tag == 8) {
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::uint32, ::google::protobuf::internal::WireFormatLite::TYPE_UINT32>(
                 input, &type_)));
          set_has_type();
        } else {
          goto handle_unusual;
        }
        if (input->ExpectTag(18)) goto parse_username;
        break;
      }

      // required string username = 2;
      case 2: {
        if (tag == 18) {
         parse_username:
          DO_(::google::protobuf::internal::WireFormatLite::ReadString(
                input, this->mutable_username()));
          ::google::protobuf::internal::WireFormat::VerifyUTF8StringNamedField(
            this->username().data(), this->username().length(),
            ::google::protobuf::internal::WireFormat::PARSE,
            "username");
        } else {
          goto handle_unusual;
        }
        if (input->ExpectTag(26)) goto parse_salaname;
        break;
      }

      // optional string salaname = 3;
      case 3: {
        if (tag == 26) {
         parse_salaname:
          DO_(::google::protobuf::internal::WireFormatLite::ReadString(
                input, this->mutable_salaname()));
          ::google::protobuf::internal::WireFormat::VerifyUTF8StringNamedField(
            this->salaname().data(), this->salaname().length(),
            ::google::protobuf::internal::WireFormat::PARSE,
            "salaname");
        } else {
          goto handle_unusual;
        }
        if (input->ExpectTag(34)) goto parse_ip;
        break;
      }

      // required string ip = 4;
      case 4: {
        if (tag == 34) {
         parse_ip:
          DO_(::google::protobuf::internal::WireFormatLite::ReadString(
                input, this->mutable_ip()));
          ::google::protobuf::internal::WireFormat::VerifyUTF8StringNamedField(
            this->ip().data(), this->ip().length(),
            ::google::protobuf::internal::WireFormat::PARSE,
            "ip");
        } else {
          goto handle_unusual;
        }
        if (input->ExpectTag(42)) goto parse_message;
        break;
      }

      // optional string message = 5;
      case 5: {
        if (tag == 42) {
         parse_message:
          DO_(::google::protobuf::internal::WireFormatLite::ReadString(
                input, this->mutable_message()));
          ::google::protobuf::internal::WireFormat::VerifyUTF8StringNamedField(
            this->message().data(), this->message().length(),
            ::google::protobuf::internal::WireFormat::PARSE,
            "message");
        } else {
          goto handle_unusual;
        }
        if (input->ExpectTag(48)) goto parse_port;
        break;
      }

      // required uint32 port = 6;
      case 6: {
        if (tag == 48) {
         parse_port:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::uint32, ::google::protobuf::internal::WireFormatLite::TYPE_UINT32>(
                 input, &port_)));
          set_has_port();
        } else {
          goto handle_unusual;
        }
        if (input->ExpectAtEnd()) goto success;
        break;
      }

      default: {
      handle_unusual:
        if (tag == 0 ||
            ::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_END_GROUP) {
          goto success;
        }
        DO_(::google::protobuf::internal::WireFormat::SkipField(
              input, tag, mutable_unknown_fields()));
        break;
      }
    }
  }
success:
  // @@protoc_insertion_point(parse_success:Message)
  return true;
failure:
  // @@protoc_insertion_point(parse_failure:Message)
  return false;
#undef DO_
}

void Message::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // @@protoc_insertion_point(serialize_start:Message)
  // required uint32 type = 1;
  if (has_type()) {
    ::google::protobuf::internal::WireFormatLite::WriteUInt32(1, this->type(), output);
  }

  // required string username = 2;
  if (has_username()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8StringNamedField(
      this->username().data(), this->username().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE,
      "username");
    ::google::protobuf::internal::WireFormatLite::WriteStringMaybeAliased(
      2, this->username(), output);
  }

  // optional string salaname = 3;
  if (has_salaname()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8StringNamedField(
      this->salaname().data(), this->salaname().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE,
      "salaname");
    ::google::protobuf::internal::WireFormatLite::WriteStringMaybeAliased(
      3, this->salaname(), output);
  }

  // required string ip = 4;
  if (has_ip()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8StringNamedField(
      this->ip().data(), this->ip().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE,
      "ip");
    ::google::protobuf::internal::WireFormatLite::WriteStringMaybeAliased(
      4, this->ip(), output);
  }

  // optional string message = 5;
  if (has_message()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8StringNamedField(
      this->message().data(), this->message().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE,
      "message");
    ::google::protobuf::internal::WireFormatLite::WriteStringMaybeAliased(
      5, this->message(), output);
  }

  // required uint32 port = 6;
  if (has_port()) {
    ::google::protobuf::internal::WireFormatLite::WriteUInt32(6, this->port(), output);
  }

  if (!unknown_fields().empty()) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        unknown_fields(), output);
  }
  // @@protoc_insertion_point(serialize_end:Message)
}

::google::protobuf::uint8* Message::SerializeWithCachedSizesToArray(
    ::google::protobuf::uint8* target) const {
  // @@protoc_insertion_point(serialize_to_array_start:Message)
  // required uint32 type = 1;
  if (has_type()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteUInt32ToArray(1, this->type(), target);
  }

  // required string username = 2;
  if (has_username()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8StringNamedField(
      this->username().data(), this->username().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE,
      "username");
    target =
      ::google::protobuf::internal::WireFormatLite::WriteStringToArray(
        2, this->username(), target);
  }

  // optional string salaname = 3;
  if (has_salaname()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8StringNamedField(
      this->salaname().data(), this->salaname().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE,
      "salaname");
    target =
      ::google::protobuf::internal::WireFormatLite::WriteStringToArray(
        3, this->salaname(), target);
  }

  // required string ip = 4;
  if (has_ip()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8StringNamedField(
      this->ip().data(), this->ip().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE,
      "ip");
    target =
      ::google::protobuf::internal::WireFormatLite::WriteStringToArray(
        4, this->ip(), target);
  }

  // optional string message = 5;
  if (has_message()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8StringNamedField(
      this->message().data(), this->message().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE,
      "message");
    target =
      ::google::protobuf::internal::WireFormatLite::WriteStringToArray(
        5, this->message(), target);
  }

  // required uint32 port = 6;
  if (has_port()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteUInt32ToArray(6, this->port(), target);
  }

  if (!unknown_fields().empty()) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        unknown_fields(), target);
  }
  // @@protoc_insertion_point(serialize_to_array_end:Message)
  return target;
}

int Message::ByteSize() const {
  int total_size = 0;

  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    // required uint32 type = 1;
    if (has_type()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::UInt32Size(
          this->type());
    }

    // required string username = 2;
    if (has_username()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::StringSize(
          this->username());
    }

    // optional string salaname = 3;
    if (has_salaname()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::StringSize(
          this->salaname());
    }

    // required string ip = 4;
    if (has_ip()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::StringSize(
          this->ip());
    }

    // optional string message = 5;
    if (has_message()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::StringSize(
          this->message());
    }

    // required uint32 port = 6;
    if (has_port()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::UInt32Size(
          this->port());
    }

  }
  if (!unknown_fields().empty()) {
    total_size +=
      ::google::protobuf::internal::WireFormat::ComputeUnknownFieldsSize(
        unknown_fields());
  }
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = total_size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
  return total_size;
}

void Message::MergeFrom(const ::google::protobuf::Message& from) {
  GOOGLE_CHECK_NE(&from, this);
  const Message* source =
    ::google::protobuf::internal::dynamic_cast_if_available<const Message*>(
      &from);
  if (source == NULL) {
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
    MergeFrom(*source);
  }
}

void Message::MergeFrom(const Message& from) {
  GOOGLE_CHECK_NE(&from, this);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from.has_type()) {
      set_type(from.type());
    }
    if (from.has_username()) {
      set_username(from.username());
    }
    if (from.has_salaname()) {
      set_salaname(from.salaname());
    }
    if (from.has_ip()) {
      set_ip(from.ip());
    }
    if (from.has_message()) {
      set_message(from.message());
    }
    if (from.has_port()) {
      set_port(from.port());
    }
  }
  mutable_unknown_fields()->MergeFrom(from.unknown_fields());
}

void Message::CopyFrom(const ::google::protobuf::Message& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void Message::CopyFrom(const Message& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool Message::IsInitialized() const {
  if ((_has_bits_[0] & 0x0000002b) != 0x0000002b) return false;

  return true;
}

void Message::Swap(Message* other) {
  if (other != this) {
    std::swap(type_, other->type_);
    std::swap(username_, other->username_);
    std::swap(salaname_, other->salaname_);
    std::swap(ip_, other->ip_);
    std::swap(message_, other->message_);
    std::swap(port_, other->port_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    _unknown_fields_.Swap(&other->_unknown_fields_);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::google::protobuf::Metadata Message::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = Message_descriptor_;
  metadata.reflection = Message_reflection_;
  return metadata;
}


// @@protoc_insertion_point(namespace_scope)

// @@protoc_insertion_point(global_scope)
