// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: lspmessage.proto

#ifndef PROTOBUF_lspmessage_2eproto__INCLUDED
#define PROTOBUF_lspmessage_2eproto__INCLUDED

#include <string>

#include <google/protobuf/stubs/common.h>

#if GOOGLE_PROTOBUF_VERSION < 2005000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please update
#error your headers.
#endif
#if 2005000 < GOOGLE_PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>
#include <google/protobuf/extension_set.h>
#include <google/protobuf/unknown_field_set.h>
// @@protoc_insertion_point(includes)

// Internal implementation detail -- do not call these.
void  protobuf_AddDesc_lspmessage_2eproto();
void protobuf_AssignDesc_lspmessage_2eproto();
void protobuf_ShutdownFile_lspmessage_2eproto();

class LSPMessage;

// ===================================================================

class LSPMessage : public ::google::protobuf::Message {
 public:
  LSPMessage();
  virtual ~LSPMessage();

  LSPMessage(const LSPMessage& from);

  inline LSPMessage& operator=(const LSPMessage& from) {
    CopyFrom(from);
    return *this;
  }

  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _unknown_fields_;
  }

  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return &_unknown_fields_;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const LSPMessage& default_instance();

  void Swap(LSPMessage* other);

  // implements Message ----------------------------------------------

  LSPMessage* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const LSPMessage& from);
  void MergeFrom(const LSPMessage& from);
  void Clear();
  bool IsInitialized() const;

  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  public:

  ::google::protobuf::Metadata GetMetadata() const;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // required uint32 connid = 1;
  inline bool has_connid() const;
  inline void clear_connid();
  static const int kConnidFieldNumber = 1;
  inline ::google::protobuf::uint32 connid() const;
  inline void set_connid(::google::protobuf::uint32 value);

  // required uint32 seqnum = 2;
  inline bool has_seqnum() const;
  inline void clear_seqnum();
  static const int kSeqnumFieldNumber = 2;
  inline ::google::protobuf::uint32 seqnum() const;
  inline void set_seqnum(::google::protobuf::uint32 value);

  // required bytes payload = 3;
  inline bool has_payload() const;
  inline void clear_payload();
  static const int kPayloadFieldNumber = 3;
  inline const ::std::string& payload() const;
  inline void set_payload(const ::std::string& value);
  inline void set_payload(const char* value);
  inline void set_payload(const void* value, size_t size);
  inline ::std::string* mutable_payload();
  inline ::std::string* release_payload();
  inline void set_allocated_payload(::std::string* payload);

  // @@protoc_insertion_point(class_scope:LSPMessage)
 private:
  inline void set_has_connid();
  inline void clear_has_connid();
  inline void set_has_seqnum();
  inline void clear_has_seqnum();
  inline void set_has_payload();
  inline void clear_has_payload();

  ::google::protobuf::UnknownFieldSet _unknown_fields_;

  ::google::protobuf::uint32 connid_;
  ::google::protobuf::uint32 seqnum_;
  ::std::string* payload_;

  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(3 + 31) / 32];

  friend void  protobuf_AddDesc_lspmessage_2eproto();
  friend void protobuf_AssignDesc_lspmessage_2eproto();
  friend void protobuf_ShutdownFile_lspmessage_2eproto();

  void InitAsDefaultInstance();
  static LSPMessage* default_instance_;
};
// ===================================================================


// ===================================================================

// LSPMessage

// required uint32 connid = 1;
inline bool LSPMessage::has_connid() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void LSPMessage::set_has_connid() {
  _has_bits_[0] |= 0x00000001u;
}
inline void LSPMessage::clear_has_connid() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void LSPMessage::clear_connid() {
  connid_ = 0u;
  clear_has_connid();
}
inline ::google::protobuf::uint32 LSPMessage::connid() const {
  return connid_;
}
inline void LSPMessage::set_connid(::google::protobuf::uint32 value) {
  set_has_connid();
  connid_ = value;
}

// required uint32 seqnum = 2;
inline bool LSPMessage::has_seqnum() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void LSPMessage::set_has_seqnum() {
  _has_bits_[0] |= 0x00000002u;
}
inline void LSPMessage::clear_has_seqnum() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void LSPMessage::clear_seqnum() {
  seqnum_ = 0u;
  clear_has_seqnum();
}
inline ::google::protobuf::uint32 LSPMessage::seqnum() const {
  return seqnum_;
}
inline void LSPMessage::set_seqnum(::google::protobuf::uint32 value) {
  set_has_seqnum();
  seqnum_ = value;
}

// required bytes payload = 3;
inline bool LSPMessage::has_payload() const {
  return (_has_bits_[0] & 0x00000004u) != 0;
}
inline void LSPMessage::set_has_payload() {
  _has_bits_[0] |= 0x00000004u;
}
inline void LSPMessage::clear_has_payload() {
  _has_bits_[0] &= ~0x00000004u;
}
inline void LSPMessage::clear_payload() {
  if (payload_ != &::google::protobuf::internal::kEmptyString) {
    payload_->clear();
  }
  clear_has_payload();
}
inline const ::std::string& LSPMessage::payload() const {
  return *payload_;
}
inline void LSPMessage::set_payload(const ::std::string& value) {
  set_has_payload();
  if (payload_ == &::google::protobuf::internal::kEmptyString) {
    payload_ = new ::std::string;
  }
  payload_->assign(value);
}
inline void LSPMessage::set_payload(const char* value) {
  set_has_payload();
  if (payload_ == &::google::protobuf::internal::kEmptyString) {
    payload_ = new ::std::string;
  }
  payload_->assign(value);
}
inline void LSPMessage::set_payload(const void* value, size_t size) {
  set_has_payload();
  if (payload_ == &::google::protobuf::internal::kEmptyString) {
    payload_ = new ::std::string;
  }
  payload_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* LSPMessage::mutable_payload() {
  set_has_payload();
  if (payload_ == &::google::protobuf::internal::kEmptyString) {
    payload_ = new ::std::string;
  }
  return payload_;
}
inline ::std::string* LSPMessage::release_payload() {
  clear_has_payload();
  if (payload_ == &::google::protobuf::internal::kEmptyString) {
    return NULL;
  } else {
    ::std::string* temp = payload_;
    payload_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
    return temp;
  }
}
inline void LSPMessage::set_allocated_payload(::std::string* payload) {
  if (payload_ != &::google::protobuf::internal::kEmptyString) {
    delete payload_;
  }
  if (payload) {
    set_has_payload();
    payload_ = payload;
  } else {
    clear_has_payload();
    payload_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
  }
}


// @@protoc_insertion_point(namespace_scope)

#ifndef SWIG
namespace google {
namespace protobuf {


}  // namespace google
}  // namespace protobuf
#endif  // SWIG

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_lspmessage_2eproto__INCLUDED
