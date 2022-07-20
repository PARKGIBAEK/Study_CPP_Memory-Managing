// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: Protocol.proto

#include "Protocol.pb.h"

#include <algorithm>

#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/extension_set.h>
#include <google/protobuf/wire_format_lite.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/reflection_ops.h>
#include <google/protobuf/wire_format.h>
// @@protoc_insertion_point(includes)
#include <google/protobuf/port_def.inc>

PROTOBUF_PRAGMA_INIT_SEG
namespace Protocol {
constexpr C_LOGIN::C_LOGIN(
  ::PROTOBUF_NAMESPACE_ID::internal::ConstantInitialized){}
struct C_LOGINDefaultTypeInternal {
  constexpr C_LOGINDefaultTypeInternal()
    : _instance(::PROTOBUF_NAMESPACE_ID::internal::ConstantInitialized{}) {}
  ~C_LOGINDefaultTypeInternal() {}
  union {
    C_LOGIN _instance;
  };
};
PROTOBUF_ATTRIBUTE_NO_DESTROY PROTOBUF_CONSTINIT C_LOGINDefaultTypeInternal _C_LOGIN_default_instance_;
constexpr S_LOGIN::S_LOGIN(
  ::PROTOBUF_NAMESPACE_ID::internal::ConstantInitialized)
  : players_()
  , success_(false){}
struct S_LOGINDefaultTypeInternal {
  constexpr S_LOGINDefaultTypeInternal()
    : _instance(::PROTOBUF_NAMESPACE_ID::internal::ConstantInitialized{}) {}
  ~S_LOGINDefaultTypeInternal() {}
  union {
    S_LOGIN _instance;
  };
};
PROTOBUF_ATTRIBUTE_NO_DESTROY PROTOBUF_CONSTINIT S_LOGINDefaultTypeInternal _S_LOGIN_default_instance_;
constexpr C_ENTER_GAME::C_ENTER_GAME(
  ::PROTOBUF_NAMESPACE_ID::internal::ConstantInitialized)
  : playerindex_(uint64_t{0u}){}
struct C_ENTER_GAMEDefaultTypeInternal {
  constexpr C_ENTER_GAMEDefaultTypeInternal()
    : _instance(::PROTOBUF_NAMESPACE_ID::internal::ConstantInitialized{}) {}
  ~C_ENTER_GAMEDefaultTypeInternal() {}
  union {
    C_ENTER_GAME _instance;
  };
};
PROTOBUF_ATTRIBUTE_NO_DESTROY PROTOBUF_CONSTINIT C_ENTER_GAMEDefaultTypeInternal _C_ENTER_GAME_default_instance_;
constexpr S_ENTER_GAME::S_ENTER_GAME(
  ::PROTOBUF_NAMESPACE_ID::internal::ConstantInitialized)
  : success_(false){}
struct S_ENTER_GAMEDefaultTypeInternal {
  constexpr S_ENTER_GAMEDefaultTypeInternal()
    : _instance(::PROTOBUF_NAMESPACE_ID::internal::ConstantInitialized{}) {}
  ~S_ENTER_GAMEDefaultTypeInternal() {}
  union {
    S_ENTER_GAME _instance;
  };
};
PROTOBUF_ATTRIBUTE_NO_DESTROY PROTOBUF_CONSTINIT S_ENTER_GAMEDefaultTypeInternal _S_ENTER_GAME_default_instance_;
constexpr C_CHAT::C_CHAT(
  ::PROTOBUF_NAMESPACE_ID::internal::ConstantInitialized){}
struct C_CHATDefaultTypeInternal {
  constexpr C_CHATDefaultTypeInternal()
    : _instance(::PROTOBUF_NAMESPACE_ID::internal::ConstantInitialized{}) {}
  ~C_CHATDefaultTypeInternal() {}
  union {
    C_CHAT _instance;
  };
};
PROTOBUF_ATTRIBUTE_NO_DESTROY PROTOBUF_CONSTINIT C_CHATDefaultTypeInternal _C_CHAT_default_instance_;
constexpr S_CHAT::S_CHAT(
  ::PROTOBUF_NAMESPACE_ID::internal::ConstantInitialized)
  : playerid_(uint64_t{0u}){}
struct S_CHATDefaultTypeInternal {
  constexpr S_CHATDefaultTypeInternal()
    : _instance(::PROTOBUF_NAMESPACE_ID::internal::ConstantInitialized{}) {}
  ~S_CHATDefaultTypeInternal() {}
  union {
    S_CHAT _instance;
  };
};
PROTOBUF_ATTRIBUTE_NO_DESTROY PROTOBUF_CONSTINIT S_CHATDefaultTypeInternal _S_CHAT_default_instance_;
}  // namespace Protocol
static ::PROTOBUF_NAMESPACE_ID::Metadata file_level_metadata_Protocol_2eproto[6];
static constexpr ::PROTOBUF_NAMESPACE_ID::EnumDescriptor const** file_level_enum_descriptors_Protocol_2eproto = nullptr;
static constexpr ::PROTOBUF_NAMESPACE_ID::ServiceDescriptor const** file_level_service_descriptors_Protocol_2eproto = nullptr;

const uint32_t TableStruct_Protocol_2eproto::offsets[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
  ~0u,  // no _has_bits_
  PROTOBUF_FIELD_OFFSET(::Protocol::C_LOGIN, _internal_metadata_),
  ~0u,  // no _extensions_
  ~0u,  // no _oneof_case_
  ~0u,  // no _weak_field_map_
  ~0u,  // no _inlined_string_donated_
  ~0u,  // no _has_bits_
  PROTOBUF_FIELD_OFFSET(::Protocol::S_LOGIN, _internal_metadata_),
  ~0u,  // no _extensions_
  ~0u,  // no _oneof_case_
  ~0u,  // no _weak_field_map_
  ~0u,  // no _inlined_string_donated_
  PROTOBUF_FIELD_OFFSET(::Protocol::S_LOGIN, success_),
  PROTOBUF_FIELD_OFFSET(::Protocol::S_LOGIN, players_),
  ~0u,  // no _has_bits_
  PROTOBUF_FIELD_OFFSET(::Protocol::C_ENTER_GAME, _internal_metadata_),
  ~0u,  // no _extensions_
  ~0u,  // no _oneof_case_
  ~0u,  // no _weak_field_map_
  ~0u,  // no _inlined_string_donated_
  PROTOBUF_FIELD_OFFSET(::Protocol::C_ENTER_GAME, playerindex_),
  ~0u,  // no _has_bits_
  PROTOBUF_FIELD_OFFSET(::Protocol::S_ENTER_GAME, _internal_metadata_),
  ~0u,  // no _extensions_
  ~0u,  // no _oneof_case_
  ~0u,  // no _weak_field_map_
  ~0u,  // no _inlined_string_donated_
  PROTOBUF_FIELD_OFFSET(::Protocol::S_ENTER_GAME, success_),
  ~0u,  // no _has_bits_
  PROTOBUF_FIELD_OFFSET(::Protocol::C_CHAT, _internal_metadata_),
  ~0u,  // no _extensions_
  ~0u,  // no _oneof_case_
  ~0u,  // no _weak_field_map_
  ~0u,  // no _inlined_string_donated_
  ~0u,  // no _has_bits_
  PROTOBUF_FIELD_OFFSET(::Protocol::S_CHAT, _internal_metadata_),
  ~0u,  // no _extensions_
  ~0u,  // no _oneof_case_
  ~0u,  // no _weak_field_map_
  ~0u,  // no _inlined_string_donated_
  PROTOBUF_FIELD_OFFSET(::Protocol::S_CHAT, playerid_),
};
static const ::PROTOBUF_NAMESPACE_ID::internal::MigrationSchema schemas[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
  { 0, -1, -1, sizeof(::Protocol::C_LOGIN)},
  { 6, -1, -1, sizeof(::Protocol::S_LOGIN)},
  { 14, -1, -1, sizeof(::Protocol::C_ENTER_GAME)},
  { 21, -1, -1, sizeof(::Protocol::S_ENTER_GAME)},
  { 28, -1, -1, sizeof(::Protocol::C_CHAT)},
  { 34, -1, -1, sizeof(::Protocol::S_CHAT)},
};

static ::PROTOBUF_NAMESPACE_ID::Message const * const file_default_instances[] = {
  reinterpret_cast<const ::PROTOBUF_NAMESPACE_ID::Message*>(&::Protocol::_C_LOGIN_default_instance_),
  reinterpret_cast<const ::PROTOBUF_NAMESPACE_ID::Message*>(&::Protocol::_S_LOGIN_default_instance_),
  reinterpret_cast<const ::PROTOBUF_NAMESPACE_ID::Message*>(&::Protocol::_C_ENTER_GAME_default_instance_),
  reinterpret_cast<const ::PROTOBUF_NAMESPACE_ID::Message*>(&::Protocol::_S_ENTER_GAME_default_instance_),
  reinterpret_cast<const ::PROTOBUF_NAMESPACE_ID::Message*>(&::Protocol::_C_CHAT_default_instance_),
  reinterpret_cast<const ::PROTOBUF_NAMESPACE_ID::Message*>(&::Protocol::_S_CHAT_default_instance_),
};

const char descriptor_table_protodef_Protocol_2eproto[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) =
  "\n\016Protocol.proto\022\010Protocol\032\nEnum.proto\032\014"
  "Struct.proto\"\t\n\007C_LOGIN\"=\n\007S_LOGIN\022\017\n\007su"
  "ccess\030\001 \001(\010\022!\n\007players\030\002 \003(\0132\020.Protocol."
  "Player\"#\n\014C_ENTER_GAME\022\023\n\013playerIndex\030\001 "
  "\001(\004\"\037\n\014S_ENTER_GAME\022\017\n\007success\030\001 \001(\010\"\010\n\006"
  "C_CHAT\"\032\n\006S_CHAT\022\020\n\010playerId\030\001 \001(\004b\006prot"
  "o3"
  ;
static const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable*const descriptor_table_Protocol_2eproto_deps[2] = {
  &::descriptor_table_Enum_2eproto,
  &::descriptor_table_Struct_2eproto,
};
static ::PROTOBUF_NAMESPACE_ID::internal::once_flag descriptor_table_Protocol_2eproto_once;
const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_Protocol_2eproto = {
  false, false, 242, descriptor_table_protodef_Protocol_2eproto, "Protocol.proto", 
  &descriptor_table_Protocol_2eproto_once, descriptor_table_Protocol_2eproto_deps, 2, 6,
  schemas, file_default_instances, TableStruct_Protocol_2eproto::offsets,
  file_level_metadata_Protocol_2eproto, file_level_enum_descriptors_Protocol_2eproto, file_level_service_descriptors_Protocol_2eproto,
};
PROTOBUF_ATTRIBUTE_WEAK const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable* descriptor_table_Protocol_2eproto_getter() {
  return &descriptor_table_Protocol_2eproto;
}

// Force running AddDescriptors() at dynamic initialization time.
PROTOBUF_ATTRIBUTE_INIT_PRIORITY static ::PROTOBUF_NAMESPACE_ID::internal::AddDescriptorsRunner dynamic_init_dummy_Protocol_2eproto(&descriptor_table_Protocol_2eproto);
namespace Protocol {

// ===================================================================

class C_LOGIN::_Internal {
 public:
};

C_LOGIN::C_LOGIN(::PROTOBUF_NAMESPACE_ID::Arena* arena,
                         bool is_message_owned)
  : ::PROTOBUF_NAMESPACE_ID::internal::ZeroFieldsBase(arena, is_message_owned) {
  // @@protoc_insertion_point(arena_constructor:Protocol.C_LOGIN)
}
C_LOGIN::C_LOGIN(const C_LOGIN& from)
  : ::PROTOBUF_NAMESPACE_ID::internal::ZeroFieldsBase() {
  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
  // @@protoc_insertion_point(copy_constructor:Protocol.C_LOGIN)
}





const ::PROTOBUF_NAMESPACE_ID::Message::ClassData C_LOGIN::_class_data_ = {
    ::PROTOBUF_NAMESPACE_ID::internal::ZeroFieldsBase::CopyImpl,
    ::PROTOBUF_NAMESPACE_ID::internal::ZeroFieldsBase::MergeImpl,
};
const ::PROTOBUF_NAMESPACE_ID::Message::ClassData*C_LOGIN::GetClassData() const { return &_class_data_; }







::PROTOBUF_NAMESPACE_ID::Metadata C_LOGIN::GetMetadata() const {
  return ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(
      &descriptor_table_Protocol_2eproto_getter, &descriptor_table_Protocol_2eproto_once,
      file_level_metadata_Protocol_2eproto[0]);
}

// ===================================================================

class S_LOGIN::_Internal {
 public:
};

void S_LOGIN::clear_players() {
  players_.Clear();
}
S_LOGIN::S_LOGIN(::PROTOBUF_NAMESPACE_ID::Arena* arena,
                         bool is_message_owned)
  : ::PROTOBUF_NAMESPACE_ID::Message(arena, is_message_owned),
  players_(arena) {
  SharedCtor();
  if (!is_message_owned) {
    RegisterArenaDtor(arena);
  }
  // @@protoc_insertion_point(arena_constructor:Protocol.S_LOGIN)
}
S_LOGIN::S_LOGIN(const S_LOGIN& from)
  : ::PROTOBUF_NAMESPACE_ID::Message(),
      players_(from.players_) {
  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
  success_ = from.success_;
  // @@protoc_insertion_point(copy_constructor:Protocol.S_LOGIN)
}

inline void S_LOGIN::SharedCtor() {
success_ = false;
}

S_LOGIN::~S_LOGIN() {
  // @@protoc_insertion_point(destructor:Protocol.S_LOGIN)
  if (GetArenaForAllocation() != nullptr) return;
  SharedDtor();
  _internal_metadata_.Delete<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
}

inline void S_LOGIN::SharedDtor() {
  GOOGLE_DCHECK(GetArenaForAllocation() == nullptr);
}

void S_LOGIN::ArenaDtor(void* object) {
  S_LOGIN* _this = reinterpret_cast< S_LOGIN* >(object);
  (void)_this;
}
void S_LOGIN::RegisterArenaDtor(::PROTOBUF_NAMESPACE_ID::Arena*) {
}
void S_LOGIN::SetCachedSize(int size) const {
  _cached_size_.Set(size);
}

void S_LOGIN::Clear() {
// @@protoc_insertion_point(message_clear_start:Protocol.S_LOGIN)
  uint32_t cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  players_.Clear();
  success_ = false;
  _internal_metadata_.Clear<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
}

const char* S_LOGIN::_InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) {
#define CHK_(x) if (PROTOBUF_PREDICT_FALSE(!(x))) goto failure
  while (!ctx->Done(&ptr)) {
    uint32_t tag;
    ptr = ::PROTOBUF_NAMESPACE_ID::internal::ReadTag(ptr, &tag);
    switch (tag >> 3) {
      // bool success = 1;
      case 1:
        if (PROTOBUF_PREDICT_TRUE(static_cast<uint8_t>(tag) == 8)) {
          success_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint64(&ptr);
          CHK_(ptr);
        } else
          goto handle_unusual;
        continue;
      // repeated .Protocol.Player players = 2;
      case 2:
        if (PROTOBUF_PREDICT_TRUE(static_cast<uint8_t>(tag) == 18)) {
          ptr -= 1;
          do {
            ptr += 1;
            ptr = ctx->ParseMessage(_internal_add_players(), ptr);
            CHK_(ptr);
            if (!ctx->DataAvailable(ptr)) break;
          } while (::PROTOBUF_NAMESPACE_ID::internal::ExpectTag<18>(ptr));
        } else
          goto handle_unusual;
        continue;
      default:
        goto handle_unusual;
    }  // switch
  handle_unusual:
    if ((tag == 0) || ((tag & 7) == 4)) {
      CHK_(ptr);
      ctx->SetLastTag(tag);
      goto message_done;
    }
    ptr = UnknownFieldParse(
        tag,
        _internal_metadata_.mutable_unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(),
        ptr, ctx);
    CHK_(ptr != nullptr);
  }  // while
message_done:
  return ptr;
failure:
  ptr = nullptr;
  goto message_done;
#undef CHK_
}

uint8_t* S_LOGIN::_InternalSerialize(
    uint8_t* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const {
  // @@protoc_insertion_point(serialize_to_array_start:Protocol.S_LOGIN)
  uint32_t cached_has_bits = 0;
  (void) cached_has_bits;

  // bool success = 1;
  if (this->_internal_success() != 0) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteBoolToArray(1, this->_internal_success(), target);
  }

  // repeated .Protocol.Player players = 2;
  for (unsigned int i = 0,
      n = static_cast<unsigned int>(this->_internal_players_size()); i < n; i++) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::
      InternalWriteMessage(2, this->_internal_players(i), target, stream);
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormat::InternalSerializeUnknownFieldsToArray(
        _internal_metadata_.unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(::PROTOBUF_NAMESPACE_ID::UnknownFieldSet::default_instance), target, stream);
  }
  // @@protoc_insertion_point(serialize_to_array_end:Protocol.S_LOGIN)
  return target;
}

size_t S_LOGIN::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:Protocol.S_LOGIN)
  size_t total_size = 0;

  uint32_t cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  // repeated .Protocol.Player players = 2;
  total_size += 1UL * this->_internal_players_size();
  for (const auto& msg : this->players_) {
    total_size +=
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::MessageSize(msg);
  }

  // bool success = 1;
  if (this->_internal_success() != 0) {
    total_size += 1 + 1;
  }

  return MaybeComputeUnknownFieldsSize(total_size, &_cached_size_);
}

const ::PROTOBUF_NAMESPACE_ID::Message::ClassData S_LOGIN::_class_data_ = {
    ::PROTOBUF_NAMESPACE_ID::Message::CopyWithSizeCheck,
    S_LOGIN::MergeImpl
};
const ::PROTOBUF_NAMESPACE_ID::Message::ClassData*S_LOGIN::GetClassData() const { return &_class_data_; }

void S_LOGIN::MergeImpl(::PROTOBUF_NAMESPACE_ID::Message* to,
                      const ::PROTOBUF_NAMESPACE_ID::Message& from) {
  static_cast<S_LOGIN *>(to)->MergeFrom(
      static_cast<const S_LOGIN &>(from));
}


void S_LOGIN::MergeFrom(const S_LOGIN& from) {
// @@protoc_insertion_point(class_specific_merge_from_start:Protocol.S_LOGIN)
  GOOGLE_DCHECK_NE(&from, this);
  uint32_t cached_has_bits = 0;
  (void) cached_has_bits;

  players_.MergeFrom(from.players_);
  if (from._internal_success() != 0) {
    _internal_set_success(from._internal_success());
  }
  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
}

void S_LOGIN::CopyFrom(const S_LOGIN& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:Protocol.S_LOGIN)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool S_LOGIN::IsInitialized() const {
  return true;
}

void S_LOGIN::InternalSwap(S_LOGIN* other) {
  using std::swap;
  _internal_metadata_.InternalSwap(&other->_internal_metadata_);
  players_.InternalSwap(&other->players_);
  swap(success_, other->success_);
}

::PROTOBUF_NAMESPACE_ID::Metadata S_LOGIN::GetMetadata() const {
  return ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(
      &descriptor_table_Protocol_2eproto_getter, &descriptor_table_Protocol_2eproto_once,
      file_level_metadata_Protocol_2eproto[1]);
}

// ===================================================================

class C_ENTER_GAME::_Internal {
 public:
};

C_ENTER_GAME::C_ENTER_GAME(::PROTOBUF_NAMESPACE_ID::Arena* arena,
                         bool is_message_owned)
  : ::PROTOBUF_NAMESPACE_ID::Message(arena, is_message_owned) {
  SharedCtor();
  if (!is_message_owned) {
    RegisterArenaDtor(arena);
  }
  // @@protoc_insertion_point(arena_constructor:Protocol.C_ENTER_GAME)
}
C_ENTER_GAME::C_ENTER_GAME(const C_ENTER_GAME& from)
  : ::PROTOBUF_NAMESPACE_ID::Message() {
  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
  playerindex_ = from.playerindex_;
  // @@protoc_insertion_point(copy_constructor:Protocol.C_ENTER_GAME)
}

inline void C_ENTER_GAME::SharedCtor() {
playerindex_ = uint64_t{0u};
}

C_ENTER_GAME::~C_ENTER_GAME() {
  // @@protoc_insertion_point(destructor:Protocol.C_ENTER_GAME)
  if (GetArenaForAllocation() != nullptr) return;
  SharedDtor();
  _internal_metadata_.Delete<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
}

inline void C_ENTER_GAME::SharedDtor() {
  GOOGLE_DCHECK(GetArenaForAllocation() == nullptr);
}

void C_ENTER_GAME::ArenaDtor(void* object) {
  C_ENTER_GAME* _this = reinterpret_cast< C_ENTER_GAME* >(object);
  (void)_this;
}
void C_ENTER_GAME::RegisterArenaDtor(::PROTOBUF_NAMESPACE_ID::Arena*) {
}
void C_ENTER_GAME::SetCachedSize(int size) const {
  _cached_size_.Set(size);
}

void C_ENTER_GAME::Clear() {
// @@protoc_insertion_point(message_clear_start:Protocol.C_ENTER_GAME)
  uint32_t cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  playerindex_ = uint64_t{0u};
  _internal_metadata_.Clear<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
}

const char* C_ENTER_GAME::_InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) {
#define CHK_(x) if (PROTOBUF_PREDICT_FALSE(!(x))) goto failure
  while (!ctx->Done(&ptr)) {
    uint32_t tag;
    ptr = ::PROTOBUF_NAMESPACE_ID::internal::ReadTag(ptr, &tag);
    switch (tag >> 3) {
      // uint64 playerIndex = 1;
      case 1:
        if (PROTOBUF_PREDICT_TRUE(static_cast<uint8_t>(tag) == 8)) {
          playerindex_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint64(&ptr);
          CHK_(ptr);
        } else
          goto handle_unusual;
        continue;
      default:
        goto handle_unusual;
    }  // switch
  handle_unusual:
    if ((tag == 0) || ((tag & 7) == 4)) {
      CHK_(ptr);
      ctx->SetLastTag(tag);
      goto message_done;
    }
    ptr = UnknownFieldParse(
        tag,
        _internal_metadata_.mutable_unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(),
        ptr, ctx);
    CHK_(ptr != nullptr);
  }  // while
message_done:
  return ptr;
failure:
  ptr = nullptr;
  goto message_done;
#undef CHK_
}

uint8_t* C_ENTER_GAME::_InternalSerialize(
    uint8_t* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const {
  // @@protoc_insertion_point(serialize_to_array_start:Protocol.C_ENTER_GAME)
  uint32_t cached_has_bits = 0;
  (void) cached_has_bits;

  // uint64 playerIndex = 1;
  if (this->_internal_playerindex() != 0) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteUInt64ToArray(1, this->_internal_playerindex(), target);
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormat::InternalSerializeUnknownFieldsToArray(
        _internal_metadata_.unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(::PROTOBUF_NAMESPACE_ID::UnknownFieldSet::default_instance), target, stream);
  }
  // @@protoc_insertion_point(serialize_to_array_end:Protocol.C_ENTER_GAME)
  return target;
}

size_t C_ENTER_GAME::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:Protocol.C_ENTER_GAME)
  size_t total_size = 0;

  uint32_t cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  // uint64 playerIndex = 1;
  if (this->_internal_playerindex() != 0) {
    total_size += ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::UInt64SizePlusOne(this->_internal_playerindex());
  }

  return MaybeComputeUnknownFieldsSize(total_size, &_cached_size_);
}

const ::PROTOBUF_NAMESPACE_ID::Message::ClassData C_ENTER_GAME::_class_data_ = {
    ::PROTOBUF_NAMESPACE_ID::Message::CopyWithSizeCheck,
    C_ENTER_GAME::MergeImpl
};
const ::PROTOBUF_NAMESPACE_ID::Message::ClassData*C_ENTER_GAME::GetClassData() const { return &_class_data_; }

void C_ENTER_GAME::MergeImpl(::PROTOBUF_NAMESPACE_ID::Message* to,
                      const ::PROTOBUF_NAMESPACE_ID::Message& from) {
  static_cast<C_ENTER_GAME *>(to)->MergeFrom(
      static_cast<const C_ENTER_GAME &>(from));
}


void C_ENTER_GAME::MergeFrom(const C_ENTER_GAME& from) {
// @@protoc_insertion_point(class_specific_merge_from_start:Protocol.C_ENTER_GAME)
  GOOGLE_DCHECK_NE(&from, this);
  uint32_t cached_has_bits = 0;
  (void) cached_has_bits;

  if (from._internal_playerindex() != 0) {
    _internal_set_playerindex(from._internal_playerindex());
  }
  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
}

void C_ENTER_GAME::CopyFrom(const C_ENTER_GAME& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:Protocol.C_ENTER_GAME)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool C_ENTER_GAME::IsInitialized() const {
  return true;
}

void C_ENTER_GAME::InternalSwap(C_ENTER_GAME* other) {
  using std::swap;
  _internal_metadata_.InternalSwap(&other->_internal_metadata_);
  swap(playerindex_, other->playerindex_);
}

::PROTOBUF_NAMESPACE_ID::Metadata C_ENTER_GAME::GetMetadata() const {
  return ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(
      &descriptor_table_Protocol_2eproto_getter, &descriptor_table_Protocol_2eproto_once,
      file_level_metadata_Protocol_2eproto[2]);
}

// ===================================================================

class S_ENTER_GAME::_Internal {
 public:
};

S_ENTER_GAME::S_ENTER_GAME(::PROTOBUF_NAMESPACE_ID::Arena* arena,
                         bool is_message_owned)
  : ::PROTOBUF_NAMESPACE_ID::Message(arena, is_message_owned) {
  SharedCtor();
  if (!is_message_owned) {
    RegisterArenaDtor(arena);
  }
  // @@protoc_insertion_point(arena_constructor:Protocol.S_ENTER_GAME)
}
S_ENTER_GAME::S_ENTER_GAME(const S_ENTER_GAME& from)
  : ::PROTOBUF_NAMESPACE_ID::Message() {
  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
  success_ = from.success_;
  // @@protoc_insertion_point(copy_constructor:Protocol.S_ENTER_GAME)
}

inline void S_ENTER_GAME::SharedCtor() {
success_ = false;
}

S_ENTER_GAME::~S_ENTER_GAME() {
  // @@protoc_insertion_point(destructor:Protocol.S_ENTER_GAME)
  if (GetArenaForAllocation() != nullptr) return;
  SharedDtor();
  _internal_metadata_.Delete<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
}

inline void S_ENTER_GAME::SharedDtor() {
  GOOGLE_DCHECK(GetArenaForAllocation() == nullptr);
}

void S_ENTER_GAME::ArenaDtor(void* object) {
  S_ENTER_GAME* _this = reinterpret_cast< S_ENTER_GAME* >(object);
  (void)_this;
}
void S_ENTER_GAME::RegisterArenaDtor(::PROTOBUF_NAMESPACE_ID::Arena*) {
}
void S_ENTER_GAME::SetCachedSize(int size) const {
  _cached_size_.Set(size);
}

void S_ENTER_GAME::Clear() {
// @@protoc_insertion_point(message_clear_start:Protocol.S_ENTER_GAME)
  uint32_t cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  success_ = false;
  _internal_metadata_.Clear<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
}

const char* S_ENTER_GAME::_InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) {
#define CHK_(x) if (PROTOBUF_PREDICT_FALSE(!(x))) goto failure
  while (!ctx->Done(&ptr)) {
    uint32_t tag;
    ptr = ::PROTOBUF_NAMESPACE_ID::internal::ReadTag(ptr, &tag);
    switch (tag >> 3) {
      // bool success = 1;
      case 1:
        if (PROTOBUF_PREDICT_TRUE(static_cast<uint8_t>(tag) == 8)) {
          success_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint64(&ptr);
          CHK_(ptr);
        } else
          goto handle_unusual;
        continue;
      default:
        goto handle_unusual;
    }  // switch
  handle_unusual:
    if ((tag == 0) || ((tag & 7) == 4)) {
      CHK_(ptr);
      ctx->SetLastTag(tag);
      goto message_done;
    }
    ptr = UnknownFieldParse(
        tag,
        _internal_metadata_.mutable_unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(),
        ptr, ctx);
    CHK_(ptr != nullptr);
  }  // while
message_done:
  return ptr;
failure:
  ptr = nullptr;
  goto message_done;
#undef CHK_
}

uint8_t* S_ENTER_GAME::_InternalSerialize(
    uint8_t* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const {
  // @@protoc_insertion_point(serialize_to_array_start:Protocol.S_ENTER_GAME)
  uint32_t cached_has_bits = 0;
  (void) cached_has_bits;

  // bool success = 1;
  if (this->_internal_success() != 0) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteBoolToArray(1, this->_internal_success(), target);
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormat::InternalSerializeUnknownFieldsToArray(
        _internal_metadata_.unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(::PROTOBUF_NAMESPACE_ID::UnknownFieldSet::default_instance), target, stream);
  }
  // @@protoc_insertion_point(serialize_to_array_end:Protocol.S_ENTER_GAME)
  return target;
}

size_t S_ENTER_GAME::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:Protocol.S_ENTER_GAME)
  size_t total_size = 0;

  uint32_t cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  // bool success = 1;
  if (this->_internal_success() != 0) {
    total_size += 1 + 1;
  }

  return MaybeComputeUnknownFieldsSize(total_size, &_cached_size_);
}

const ::PROTOBUF_NAMESPACE_ID::Message::ClassData S_ENTER_GAME::_class_data_ = {
    ::PROTOBUF_NAMESPACE_ID::Message::CopyWithSizeCheck,
    S_ENTER_GAME::MergeImpl
};
const ::PROTOBUF_NAMESPACE_ID::Message::ClassData*S_ENTER_GAME::GetClassData() const { return &_class_data_; }

void S_ENTER_GAME::MergeImpl(::PROTOBUF_NAMESPACE_ID::Message* to,
                      const ::PROTOBUF_NAMESPACE_ID::Message& from) {
  static_cast<S_ENTER_GAME *>(to)->MergeFrom(
      static_cast<const S_ENTER_GAME &>(from));
}


void S_ENTER_GAME::MergeFrom(const S_ENTER_GAME& from) {
// @@protoc_insertion_point(class_specific_merge_from_start:Protocol.S_ENTER_GAME)
  GOOGLE_DCHECK_NE(&from, this);
  uint32_t cached_has_bits = 0;
  (void) cached_has_bits;

  if (from._internal_success() != 0) {
    _internal_set_success(from._internal_success());
  }
  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
}

void S_ENTER_GAME::CopyFrom(const S_ENTER_GAME& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:Protocol.S_ENTER_GAME)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool S_ENTER_GAME::IsInitialized() const {
  return true;
}

void S_ENTER_GAME::InternalSwap(S_ENTER_GAME* other) {
  using std::swap;
  _internal_metadata_.InternalSwap(&other->_internal_metadata_);
  swap(success_, other->success_);
}

::PROTOBUF_NAMESPACE_ID::Metadata S_ENTER_GAME::GetMetadata() const {
  return ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(
      &descriptor_table_Protocol_2eproto_getter, &descriptor_table_Protocol_2eproto_once,
      file_level_metadata_Protocol_2eproto[3]);
}

// ===================================================================

class C_CHAT::_Internal {
 public:
};

C_CHAT::C_CHAT(::PROTOBUF_NAMESPACE_ID::Arena* arena,
                         bool is_message_owned)
  : ::PROTOBUF_NAMESPACE_ID::internal::ZeroFieldsBase(arena, is_message_owned) {
  // @@protoc_insertion_point(arena_constructor:Protocol.C_CHAT)
}
C_CHAT::C_CHAT(const C_CHAT& from)
  : ::PROTOBUF_NAMESPACE_ID::internal::ZeroFieldsBase() {
  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
  // @@protoc_insertion_point(copy_constructor:Protocol.C_CHAT)
}





const ::PROTOBUF_NAMESPACE_ID::Message::ClassData C_CHAT::_class_data_ = {
    ::PROTOBUF_NAMESPACE_ID::internal::ZeroFieldsBase::CopyImpl,
    ::PROTOBUF_NAMESPACE_ID::internal::ZeroFieldsBase::MergeImpl,
};
const ::PROTOBUF_NAMESPACE_ID::Message::ClassData*C_CHAT::GetClassData() const { return &_class_data_; }







::PROTOBUF_NAMESPACE_ID::Metadata C_CHAT::GetMetadata() const {
  return ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(
      &descriptor_table_Protocol_2eproto_getter, &descriptor_table_Protocol_2eproto_once,
      file_level_metadata_Protocol_2eproto[4]);
}

// ===================================================================

class S_CHAT::_Internal {
 public:
};

S_CHAT::S_CHAT(::PROTOBUF_NAMESPACE_ID::Arena* arena,
                         bool is_message_owned)
  : ::PROTOBUF_NAMESPACE_ID::Message(arena, is_message_owned) {
  SharedCtor();
  if (!is_message_owned) {
    RegisterArenaDtor(arena);
  }
  // @@protoc_insertion_point(arena_constructor:Protocol.S_CHAT)
}
S_CHAT::S_CHAT(const S_CHAT& from)
  : ::PROTOBUF_NAMESPACE_ID::Message() {
  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
  playerid_ = from.playerid_;
  // @@protoc_insertion_point(copy_constructor:Protocol.S_CHAT)
}

inline void S_CHAT::SharedCtor() {
playerid_ = uint64_t{0u};
}

S_CHAT::~S_CHAT() {
  // @@protoc_insertion_point(destructor:Protocol.S_CHAT)
  if (GetArenaForAllocation() != nullptr) return;
  SharedDtor();
  _internal_metadata_.Delete<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
}

inline void S_CHAT::SharedDtor() {
  GOOGLE_DCHECK(GetArenaForAllocation() == nullptr);
}

void S_CHAT::ArenaDtor(void* object) {
  S_CHAT* _this = reinterpret_cast< S_CHAT* >(object);
  (void)_this;
}
void S_CHAT::RegisterArenaDtor(::PROTOBUF_NAMESPACE_ID::Arena*) {
}
void S_CHAT::SetCachedSize(int size) const {
  _cached_size_.Set(size);
}

void S_CHAT::Clear() {
// @@protoc_insertion_point(message_clear_start:Protocol.S_CHAT)
  uint32_t cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  playerid_ = uint64_t{0u};
  _internal_metadata_.Clear<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
}

const char* S_CHAT::_InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) {
#define CHK_(x) if (PROTOBUF_PREDICT_FALSE(!(x))) goto failure
  while (!ctx->Done(&ptr)) {
    uint32_t tag;
    ptr = ::PROTOBUF_NAMESPACE_ID::internal::ReadTag(ptr, &tag);
    switch (tag >> 3) {
      // uint64 playerId = 1;
      case 1:
        if (PROTOBUF_PREDICT_TRUE(static_cast<uint8_t>(tag) == 8)) {
          playerid_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint64(&ptr);
          CHK_(ptr);
        } else
          goto handle_unusual;
        continue;
      default:
        goto handle_unusual;
    }  // switch
  handle_unusual:
    if ((tag == 0) || ((tag & 7) == 4)) {
      CHK_(ptr);
      ctx->SetLastTag(tag);
      goto message_done;
    }
    ptr = UnknownFieldParse(
        tag,
        _internal_metadata_.mutable_unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(),
        ptr, ctx);
    CHK_(ptr != nullptr);
  }  // while
message_done:
  return ptr;
failure:
  ptr = nullptr;
  goto message_done;
#undef CHK_
}

uint8_t* S_CHAT::_InternalSerialize(
    uint8_t* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const {
  // @@protoc_insertion_point(serialize_to_array_start:Protocol.S_CHAT)
  uint32_t cached_has_bits = 0;
  (void) cached_has_bits;

  // uint64 playerId = 1;
  if (this->_internal_playerid() != 0) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteUInt64ToArray(1, this->_internal_playerid(), target);
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormat::InternalSerializeUnknownFieldsToArray(
        _internal_metadata_.unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(::PROTOBUF_NAMESPACE_ID::UnknownFieldSet::default_instance), target, stream);
  }
  // @@protoc_insertion_point(serialize_to_array_end:Protocol.S_CHAT)
  return target;
}

size_t S_CHAT::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:Protocol.S_CHAT)
  size_t total_size = 0;

  uint32_t cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  // uint64 playerId = 1;
  if (this->_internal_playerid() != 0) {
    total_size += ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::UInt64SizePlusOne(this->_internal_playerid());
  }

  return MaybeComputeUnknownFieldsSize(total_size, &_cached_size_);
}

const ::PROTOBUF_NAMESPACE_ID::Message::ClassData S_CHAT::_class_data_ = {
    ::PROTOBUF_NAMESPACE_ID::Message::CopyWithSizeCheck,
    S_CHAT::MergeImpl
};
const ::PROTOBUF_NAMESPACE_ID::Message::ClassData*S_CHAT::GetClassData() const { return &_class_data_; }

void S_CHAT::MergeImpl(::PROTOBUF_NAMESPACE_ID::Message* to,
                      const ::PROTOBUF_NAMESPACE_ID::Message& from) {
  static_cast<S_CHAT *>(to)->MergeFrom(
      static_cast<const S_CHAT &>(from));
}


void S_CHAT::MergeFrom(const S_CHAT& from) {
// @@protoc_insertion_point(class_specific_merge_from_start:Protocol.S_CHAT)
  GOOGLE_DCHECK_NE(&from, this);
  uint32_t cached_has_bits = 0;
  (void) cached_has_bits;

  if (from._internal_playerid() != 0) {
    _internal_set_playerid(from._internal_playerid());
  }
  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
}

void S_CHAT::CopyFrom(const S_CHAT& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:Protocol.S_CHAT)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool S_CHAT::IsInitialized() const {
  return true;
}

void S_CHAT::InternalSwap(S_CHAT* other) {
  using std::swap;
  _internal_metadata_.InternalSwap(&other->_internal_metadata_);
  swap(playerid_, other->playerid_);
}

::PROTOBUF_NAMESPACE_ID::Metadata S_CHAT::GetMetadata() const {
  return ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(
      &descriptor_table_Protocol_2eproto_getter, &descriptor_table_Protocol_2eproto_once,
      file_level_metadata_Protocol_2eproto[5]);
}

// @@protoc_insertion_point(namespace_scope)
}  // namespace Protocol
PROTOBUF_NAMESPACE_OPEN
template<> PROTOBUF_NOINLINE ::Protocol::C_LOGIN* Arena::CreateMaybeMessage< ::Protocol::C_LOGIN >(Arena* arena) {
  return Arena::CreateMessageInternal< ::Protocol::C_LOGIN >(arena);
}
template<> PROTOBUF_NOINLINE ::Protocol::S_LOGIN* Arena::CreateMaybeMessage< ::Protocol::S_LOGIN >(Arena* arena) {
  return Arena::CreateMessageInternal< ::Protocol::S_LOGIN >(arena);
}
template<> PROTOBUF_NOINLINE ::Protocol::C_ENTER_GAME* Arena::CreateMaybeMessage< ::Protocol::C_ENTER_GAME >(Arena* arena) {
  return Arena::CreateMessageInternal< ::Protocol::C_ENTER_GAME >(arena);
}
template<> PROTOBUF_NOINLINE ::Protocol::S_ENTER_GAME* Arena::CreateMaybeMessage< ::Protocol::S_ENTER_GAME >(Arena* arena) {
  return Arena::CreateMessageInternal< ::Protocol::S_ENTER_GAME >(arena);
}
template<> PROTOBUF_NOINLINE ::Protocol::C_CHAT* Arena::CreateMaybeMessage< ::Protocol::C_CHAT >(Arena* arena) {
  return Arena::CreateMessageInternal< ::Protocol::C_CHAT >(arena);
}
template<> PROTOBUF_NOINLINE ::Protocol::S_CHAT* Arena::CreateMaybeMessage< ::Protocol::S_CHAT >(Arena* arena) {
  return Arena::CreateMessageInternal< ::Protocol::S_CHAT >(arena);
}
PROTOBUF_NAMESPACE_CLOSE

// @@protoc_insertion_point(global_scope)
#include <google/protobuf/port_undef.inc>
