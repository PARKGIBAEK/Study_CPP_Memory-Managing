// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: Struct.proto

#ifndef GOOGLE_PROTOBUF_INCLUDED_Struct_2eproto
#define GOOGLE_PROTOBUF_INCLUDED_Struct_2eproto

#include <limits>
#include <string>

#include <google/protobuf/port_def.inc>
#if PROTOBUF_VERSION < 3019000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers. Please update
#error your headers.
#endif
#if 3019004 < PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers. Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/port_undef.inc>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/arena.h>
#include <google/protobuf/arenastring.h>
#include <google/protobuf/generated_message_table_driven.h>
#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/metadata_lite.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>  // IWYU pragma: export
#include <google/protobuf/extension_set.h>  // IWYU pragma: export
#include <google/protobuf/unknown_field_set.h>
#include "Enum.pb.h"
// @@protoc_insertion_point(includes)
#include <google/protobuf/port_def.inc>
#define PROTOBUF_INTERNAL_EXPORT_Struct_2eproto
PROTOBUF_NAMESPACE_OPEN
namespace internal {
class AnyMetadata;
}  // namespace internal
PROTOBUF_NAMESPACE_CLOSE

// Internal implementation detail -- do not use these members.
struct TableStruct_Struct_2eproto {
  static const ::PROTOBUF_NAMESPACE_ID::internal::ParseTableField entries[]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::AuxiliaryParseTableField aux[]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::ParseTable schema[1]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::FieldMetadata field_metadata[];
  static const ::PROTOBUF_NAMESPACE_ID::internal::SerializationTable serialization_table[];
  static const uint32_t offsets[];
};
extern const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_Struct_2eproto;
namespace Protocol {
class Player;
struct PlayerDefaultTypeInternal;
extern PlayerDefaultTypeInternal _Player_default_instance_;
}  // namespace Protocol
PROTOBUF_NAMESPACE_OPEN
template<> ::Protocol::Player* Arena::CreateMaybeMessage<::Protocol::Player>(Arena*);
PROTOBUF_NAMESPACE_CLOSE
namespace Protocol {

// ===================================================================

class Player final :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:Protocol.Player) */ {
 public:
  inline Player() : Player(nullptr) {}
  ~Player() override;
  explicit constexpr Player(::PROTOBUF_NAMESPACE_ID::internal::ConstantInitialized);

  Player(const Player& from);
  Player(Player&& from) noexcept
    : Player() {
    *this = ::std::move(from);
  }

  inline Player& operator=(const Player& from) {
    CopyFrom(from);
    return *this;
  }
  inline Player& operator=(Player&& from) noexcept {
    if (this == &from) return *this;
    if (GetOwningArena() == from.GetOwningArena()
  #ifdef PROTOBUF_FORCE_COPY_IN_MOVE
        && GetOwningArena() != nullptr
  #endif  // !PROTOBUF_FORCE_COPY_IN_MOVE
    ) {
      InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }

  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* descriptor() {
    return GetDescriptor();
  }
  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* GetDescriptor() {
    return default_instance().GetMetadata().descriptor;
  }
  static const ::PROTOBUF_NAMESPACE_ID::Reflection* GetReflection() {
    return default_instance().GetMetadata().reflection;
  }
  static const Player& default_instance() {
    return *internal_default_instance();
  }
  static inline const Player* internal_default_instance() {
    return reinterpret_cast<const Player*>(
               &_Player_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    0;

  friend void swap(Player& a, Player& b) {
    a.Swap(&b);
  }
  inline void Swap(Player* other) {
    if (other == this) return;
  #ifdef PROTOBUF_FORCE_COPY_IN_SWAP
    if (GetOwningArena() != nullptr &&
        GetOwningArena() == other->GetOwningArena()) {
   #else  // PROTOBUF_FORCE_COPY_IN_SWAP
    if (GetOwningArena() == other->GetOwningArena()) {
  #endif  // !PROTOBUF_FORCE_COPY_IN_SWAP
      InternalSwap(other);
    } else {
      ::PROTOBUF_NAMESPACE_ID::internal::GenericSwap(this, other);
    }
  }
  void UnsafeArenaSwap(Player* other) {
    if (other == this) return;
    GOOGLE_DCHECK(GetOwningArena() == other->GetOwningArena());
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  Player* New(::PROTOBUF_NAMESPACE_ID::Arena* arena = nullptr) const final {
    return CreateMaybeMessage<Player>(arena);
  }
  using ::PROTOBUF_NAMESPACE_ID::Message::CopyFrom;
  void CopyFrom(const Player& from);
  using ::PROTOBUF_NAMESPACE_ID::Message::MergeFrom;
  void MergeFrom(const Player& from);
  private:
  static void MergeImpl(::PROTOBUF_NAMESPACE_ID::Message* to, const ::PROTOBUF_NAMESPACE_ID::Message& from);
  public:
  PROTOBUF_ATTRIBUTE_REINITIALIZES void Clear() final;
  bool IsInitialized() const final;

  size_t ByteSizeLong() const final;
  const char* _InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) final;
  uint8_t* _InternalSerialize(
      uint8_t* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const final;
  int GetCachedSize() const final { return _cached_size_.Get(); }

  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const final;
  void InternalSwap(Player* other);

  private:
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "Protocol.Player";
  }
  protected:
  explicit Player(::PROTOBUF_NAMESPACE_ID::Arena* arena,
                       bool is_message_owned = false);
  private:
  static void ArenaDtor(void* object);
  inline void RegisterArenaDtor(::PROTOBUF_NAMESPACE_ID::Arena* arena);
  public:

  static const ClassData _class_data_;
  const ::PROTOBUF_NAMESPACE_ID::Message::ClassData*GetClassData() const final;

  ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadata() const final;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  enum : int {
    kIdFieldNumber = 1,
    kPlayerTypeFieldNumber = 3,
  };
  // uint64 id = 1;
  void clear_id();
  uint64_t id() const;
  void set_id(uint64_t value);
  private:
  uint64_t _internal_id() const;
  void _internal_set_id(uint64_t value);
  public:

  // .Protocol.PlayerType playerType = 3;
  void clear_playertype();
  ::Protocol::PlayerType playertype() const;
  void set_playertype(::Protocol::PlayerType value);
  private:
  ::Protocol::PlayerType _internal_playertype() const;
  void _internal_set_playertype(::Protocol::PlayerType value);
  public:

  // @@protoc_insertion_point(class_scope:Protocol.Player)
 private:
  class _Internal;

  template <typename T> friend class ::PROTOBUF_NAMESPACE_ID::Arena::InternalHelper;
  typedef void InternalArenaConstructable_;
  typedef void DestructorSkippable_;
  uint64_t id_;
  int playertype_;
  mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  friend struct ::TableStruct_Struct_2eproto;
};
// ===================================================================


// ===================================================================

#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
// Player

// uint64 id = 1;
inline void Player::clear_id() {
  id_ = uint64_t{0u};
}
inline uint64_t Player::_internal_id() const {
  return id_;
}
inline uint64_t Player::id() const {
  // @@protoc_insertion_point(field_get:Protocol.Player.id)
  return _internal_id();
}
inline void Player::_internal_set_id(uint64_t value) {
  
  id_ = value;
}
inline void Player::set_id(uint64_t value) {
  _internal_set_id(value);
  // @@protoc_insertion_point(field_set:Protocol.Player.id)
}

// .Protocol.PlayerType playerType = 3;
inline void Player::clear_playertype() {
  playertype_ = 0;
}
inline ::Protocol::PlayerType Player::_internal_playertype() const {
  return static_cast< ::Protocol::PlayerType >(playertype_);
}
inline ::Protocol::PlayerType Player::playertype() const {
  // @@protoc_insertion_point(field_get:Protocol.Player.playerType)
  return _internal_playertype();
}
inline void Player::_internal_set_playertype(::Protocol::PlayerType value) {
  
  playertype_ = value;
}
inline void Player::set_playertype(::Protocol::PlayerType value) {
  _internal_set_playertype(value);
  // @@protoc_insertion_point(field_set:Protocol.Player.playerType)
}

#ifdef __GNUC__
  #pragma GCC diagnostic pop
#endif  // __GNUC__

// @@protoc_insertion_point(namespace_scope)

}  // namespace Protocol

// @@protoc_insertion_point(global_scope)

#include <google/protobuf/port_undef.inc>
#endif  // GOOGLE_PROTOBUF_INCLUDED_GOOGLE_PROTOBUF_INCLUDED_Struct_2eproto
