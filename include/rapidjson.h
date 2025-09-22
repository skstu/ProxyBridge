#if !defined(__BE568688_DF41_4AE6_8154_49C6B7EEDDF4__)
#define __BE568688_DF41_4AE6_8154_49C6B7EEDDF4__

#include "../third_party/rapidjson/include/rapidjson/document.h"
#include "../third_party/rapidjson/include/rapidjson/rapidjson.h"
#include "../third_party/rapidjson/include/rapidjson/reader.h"
#include "../third_party/rapidjson/include/rapidjson/stream.h"
#include "../third_party/rapidjson/include/rapidjson/stringbuffer.h"
#include "../third_party/rapidjson/include/rapidjson/writer.h"

#define RAPIDJSON_ADDMEMBER_STRING(obj, key, value, allocator)                 \
  obj.AddMember(rapidjson::Value()                                             \
                    .SetString(key.empty() ? "" : key.c_str(), allocator)      \
                    .Move(),                                                   \
                rapidjson::Value()                                             \
                    .SetString(value.empty() ? "" : value.c_str(), allocator)  \
                    .Move(),                                                   \
                allocator);

#define RAPIDJSON_ADDMEMBER_OBJECT(obj, key, value, allocator)                 \
  obj.AddMember(rapidjson::Value()                                             \
                    .SetString(key.empty() ? "" : key.c_str(), allocator)      \
                    .Move(),                                                   \
                value, allocator);

#define RAPIDJSON_ADDMEMBER_BOOL(obj, key, value, allocator)                   \
  obj.AddMember(rapidjson::Value()                                             \
                    .SetString(key.empty() ? "" : key.c_str(), allocator)      \
                    .Move(),                                                   \
                rapidjson::Value().SetBool(value).Move(), allocator);

#define RAPIDJSON_ADDMEMBER_INT(obj, key, value, allocator)                    \
  obj.AddMember(rapidjson::Value()                                             \
                    .SetString(key.empty() ? "" : key.c_str(), allocator)      \
                    .Move(),                                                   \
                rapidjson::Value().SetInt(value).Move(), allocator);

#define RAPIDJSON_ADDMEMBER_UINT(obj, key, value, allocator)                   \
  obj.AddMember(rapidjson::Value()                                             \
                    .SetString(key.empty() ? "" : key.c_str(), allocator)      \
                    .Move(),                                                   \
                rapidjson::Value().SetUint(value).Move(), allocator);

#define RAPIDJSON_ADDMEMBER_INT64(obj, key, value, allocator)                  \
  obj.AddMember(rapidjson::Value()                                             \
                    .SetString(key.empty() ? "" : key.c_str(), allocator)      \
                    .Move(),                                                   \
                rapidjson::Value().SetInt64(value).Move(), allocator);

#define RAPIDJSON_ADDMEMBER_UINT64(obj, key, value, allocator)                 \
  obj.AddMember(rapidjson::Value()                                             \
                    .SetString(key.empty() ? "" : key.c_str(), allocator)      \
                    .Move(),                                                   \
                rapidjson::Value().SetUint64(value).Move(), allocator);

#define RAPIDJSON_ADDMEMBER_FLOAT(obj, key, value, allocator)                  \
  obj.AddMember(rapidjson::Value()                                             \
                    .SetString(key.empty() ? "" : key.c_str(), allocator)      \
                    .Move(),                                                   \
                rapidjson::Value().SetFloat(value).Move(), allocator);

#define RAPIDJSON_ADDMEMBER_DOUBLE(obj, key, value, allocator)                 \
  obj.AddMember(rapidjson::Value()                                             \
                    .SetString(key.empty() ? "" : key.c_str(), allocator)      \
                    .Move(),                                                   \
                rapidjson::Value().SetDouble(value).Move(), allocator);

#define JSON_VALUE_TO_STRING(in, out)                                          \
  if (in.IsInt())                                                              \
    out = std::to_string(in.GetInt());                                         \
  else if (in.IsUint())                                                        \
    out = std::to_string(in.GetUint());                                        \
  else if (in.IsUint64())                                                      \
    out = std::to_string(in.GetUint64());                                      \
  else if (in.IsInt64())                                                       \
    out = std::to_string(in.GetInt64());                                       \
  else if (in.IsFalse())                                                       \
    out = std::to_string(in.GetFloat());                                       \
  else if (in.IsDouble())                                                      \
    out = std::to_string(in.GetDouble());                                      \
  else if (in.IsBool())                                                        \
    out = in.GetBool() ? "1" : "0";                                            \
  else if (in.IsString())                                                      \
    out = std::string(in.GetString(), in.GetStringLength());

#define JSON_VALUE_FIXED(in, out)                                              \
  out = in;                                                                    \
  if (!out.empty())                                                            \
    for (auto it = out.begin(); it != out.end(); ++it)                         \
      if (*it == ':')                                                          \
        *it = '_';                                                             \
      else if (*it == '\\')                                                    \
        *it = '_';                                                             \
      else if (*it == '/')                                                     \
        *it = '_';                                                             \
      else if (*it == ',')                                                     \
        *it = ' ';                                                             \
      else if (*it == '{')                                                     \
        *it = '(';                                                             \
      else if (*it == '}')                                                     \
        *it = ')';                                                             \
      else if (*it == '<')                                                     \
        *it = '(';                                                             \
      else if (*it == '>')                                                     \
        *it = ')';                                                             \
      else if (*it == '[')                                                     \
        *it = '(';                                                             \
      else if (*it == ']')                                                     \
        *it = ')';

static bool operator<(const rapidjson::Value& obj1,
 const rapidjson::Value& obj2) {
 bool result = false;
 if (obj1.GetType() != obj2.GetType())
  return false;
 switch (obj1.GetType()) {
 case rapidjson::Type::kFalseType:
 case rapidjson::Type::kTrueType: {
  return obj1.GetBool() < obj2.GetBool();
 } break;
 case rapidjson::Type::kStringType: {
  return std::string(obj1.GetString(), obj1.GetStringLength()) <
   std::string(obj2.GetString(), obj2.GetStringLength());
 } break;
 case rapidjson::Type::kNumberType: {
  if (obj1.IsDouble())
   return obj1.GetDouble() < obj2.GetDouble();
  else if (obj1.IsUint())
   return obj1.GetUint() < obj2.GetUint();
  else if (obj1.IsInt())
   return obj1.GetInt() < obj2.GetInt();
  else if (obj1.IsFloat())
   return obj1.GetFloat() < obj2.GetFloat();
  else if (obj1.IsUint64())
   return obj1.GetUint64() < obj2.GetUint64();
  else if (obj1.IsInt64())
   return obj1.GetInt64() < obj2.GetInt64();
 } break;
 case rapidjson::Type::kObjectType:
 case rapidjson::Type::kArrayType:
 case rapidjson::Type::kNullType:
 default: {
 } break;
 }
 return false;
}

static bool operator>(const rapidjson::Value& obj1,
 const rapidjson::Value& obj2) {
 bool result = false;
 if (obj1.GetType() != obj2.GetType())
  return false;
 switch (obj1.GetType()) {
 case rapidjson::Type::kFalseType:
 case rapidjson::Type::kTrueType: {
  return obj1.GetBool() > obj2.GetBool();
 } break;
 case rapidjson::Type::kStringType: {
  return std::string(obj1.GetString(), obj1.GetStringLength()) >
   std::string(obj2.GetString(), obj2.GetStringLength());
 } break;
 case rapidjson::Type::kNumberType: {
  if (obj1.IsDouble())
   return obj1.GetDouble() > obj2.GetDouble();
  else if (obj1.IsUint())
   return obj1.GetUint() > obj2.GetUint();
  else if (obj1.IsInt())
   return obj1.GetInt() > obj2.GetInt();
  else if (obj1.IsFloat())
   return obj1.GetFloat() > obj2.GetFloat();
  else if (obj1.IsUint64())
   return obj1.GetUint64() > obj2.GetUint64();
  else if (obj1.IsInt64())
   return obj1.GetInt64() > obj2.GetInt64();
 } break;
 case rapidjson::Type::kObjectType:
 case rapidjson::Type::kArrayType:
 case rapidjson::Type::kNullType:
 default: {
 } break;
 }
 return false;
}

static bool operator==(const rapidjson::Value& obj1,
 const rapidjson::Value& obj2) {
 bool result = false;
 if (obj1.GetType() != obj2.GetType())
  return false;
 switch (obj1.GetType()) {
 case rapidjson::Type::kFalseType:
 case rapidjson::Type::kTrueType: {
  return obj1.GetBool() == obj2.GetBool();
 } break;
 case rapidjson::Type::kStringType: {
  return std::string(obj1.GetString(), obj1.GetStringLength()) ==
   std::string(obj2.GetString(), obj2.GetStringLength());
 } break;
 case rapidjson::Type::kNumberType: {
  if (obj1.IsDouble())
   return obj1.GetDouble() == obj2.GetDouble();
  else if (obj1.IsUint())
   return obj1.GetUint() == obj2.GetUint();
  else if (obj1.IsInt())
   return obj1.GetInt() == obj2.GetInt();
  else if (obj1.IsFloat())
   return obj1.GetFloat() == obj2.GetFloat();
  else if (obj1.IsUint64())
   return obj1.GetUint64() == obj2.GetUint64();
  else if (obj1.IsInt64())
   return obj1.GetInt64() == obj2.GetInt64();
 } break;
 case rapidjson::Type::kObjectType:
 case rapidjson::Type::kArrayType:
 case rapidjson::Type::kNullType:
 default: {
 } break;
 }
 return false;
}

class Json final {
public:
 class Value {
 public:
  enum class ValueType : int {
   Null = 0,
   String = 1,
   Uint = 2,
   Int = 3,
   Uint64 = 4,
   Int64 = 5,
   Double = 6,
   Float = 7,
   Bool = 8,
  };

 public:
  Value() {
   source_.clear();
   type_ = ValueType::Null;
  }
  Value(const rapidjson::Value& value) {
   *this << value;
  }

 public:
  void operator=(const rapidjson::Value& value) {
   *this << value;
  }
  void operator=(const Value& obj) {
   type_ = obj.type_;
   source_ = obj.source_;
  }
  const ValueType& Type() const {
   return type_;
  }
  const std::string& Source() const {
   return source_;
  }

 private:
  void operator<<(const rapidjson::Value& value) {
   const auto raptype_ = value.GetType();
   switch (raptype_) {
   case rapidjson::Type::kStringType: {
    type_ = ValueType::String;
    source_ = value.GetStringLength() > 0 ? value.GetString() : "";
   } break;
   case rapidjson::Type::kTrueType:
   case rapidjson::Type::kFalseType: {
    type_ = ValueType::Bool;
    bool v = value.GetBool();
    source_.append((char*)&v, sizeof(v));
   } break;
   case rapidjson::Type::kNumberType: {
    if (value.IsDouble()) {
     type_ = ValueType::Double;
     double v = value.GetDouble();
     source_.append((char*)&v, sizeof(v));
    }
    else if (value.IsFloat()) {
     type_ = ValueType::Float;
     float v = value.GetFloat();
     source_.append((char*)&v, sizeof(v));
    }
    else if (value.IsInt()) {
     type_ = ValueType::Int;
     std::int32_t v = value.GetInt();
     source_.append((char*)&v, sizeof(v));
    }
    else if (value.IsInt64()) {
     type_ = ValueType::Int64;
     std::int64_t v = value.GetInt64();
     source_.append((char*)&v, sizeof(v));
    }
    else if (value.IsUint()) {
     type_ = ValueType::Uint;
     std::uint32_t v = value.GetUint();
     source_.append((char*)&v, sizeof(v));
    }
    else if (value.IsUint64()) {
     type_ = ValueType::Uint64;
     std::uint64_t v = value.GetUint64();
     source_.append((char*)&v, sizeof(v));
    }
    else {
     type_ = ValueType::Null;
     source_.clear();
    }
   } break;
   case rapidjson::Type::kNullType:
   case rapidjson::Type::kObjectType:
   case rapidjson::Type::kArrayType:
   default: {
    type_ = ValueType::Null;
    source_.clear();
   } break;
   }
  }

 private:
  ValueType type_;
  std::string source_;
 };

 typedef std::map<std::string, Json::Value> tfHttpGetNode;

 static void MadeHttpGetNode(const rapidjson::Value& value,
  const std::string& path, tfHttpGetNode& nodes) {
  if (value.IsObject()) {
   for (auto itr = value.MemberBegin(); itr != value.MemberEnd(); ++itr) {
    std::string newPath = path.empty() ? itr->name.GetString()
     : path + "." + itr->name.GetString();
    MadeHttpGetNode(itr->value, newPath, nodes);
   }
  }
  else if (value.IsArray()) {
   for (rapidjson::SizeType i = 0; i < value.Size(); ++i) {
    std::string newPath = path + "[" + std::to_string(i) + "]";
    MadeHttpGetNode(value[i], newPath, nodes);
   }
  }
  else {
   nodes.emplace(path, value);
  }
 }

 // 返回键名和索引，如果无法解析则返回 std::nullopt
 static std::optional<std::tuple<std::string, int>>
  extractKeyAndIndex(const std::string& part) {
  std::regex re(R"((\w+)\[(\d+)\])"); // 匹配格式：key[0]
  std::smatch match;
  if (std::regex_match(part, match, re) && match.size() > 2) {
   std::string key = match[1].str();      // 提取键名
   int index = std::stoi(match[2].str()); // 提取数组索引
   return std::make_tuple(key, index);
  }
  return std::nullopt; // 表示解析失败
 }

 static std::vector<std::string> StringSpilt(const std::string& input,
  const std::string& delim) {
  std::vector<std::string> result;
  do {
   if (input.empty()) {
    break;
   }
   if (delim.empty()) {
    result.emplace_back(input);
    break;
   }
   std::string escaped_delim =
    regex_replace(delim, std::regex(R"([\.\+\*\?\[\^\$\(\|])"), R"(\$&)");
   std::regex re(escaped_delim);
   result = std::vector<std::string>{
       std::sregex_token_iterator(input.begin(), input.end(), re, -1),
       std::sregex_token_iterator() };
  } while (0);
  return result;
 }
 //!@ TODO: This method has yet to be improved !
 static void UnMadeHttpGetNode(const tfHttpGetNode& nodes, std::string& out) {
  out.clear();
  if (nodes.empty()) {
   out = "{}";
   return;
  }

  rapidjson::Document doc;
  bool isObject = false;

  // 判断顶层是对象还是数组
  for (const auto& node : nodes) {
   if (node.first.find("[") == std::string::npos &&
    node.first.find("]") == std::string::npos) {
    isObject = true;
    break;
   }
  }
  isObject ? doc.SetObject() : doc.SetArray();

  // 遍历所有节点
  for (const auto& node : nodes) {
   rapidjson::Value* current = &doc;
   std::vector<std::string> tokens = Json::StringSpilt(node.first, ".");

   for (size_t i = 0; i < tokens.size(); ++i) {
    const std::string& part = tokens[i];
    auto result = extractKeyAndIndex(part);
    if (0 /*result*/) {
     auto [key, index] = result.value();
     if (!current->HasMember(key.c_str())) {
      current->AddMember(
       rapidjson::Value(key.c_str(), doc.GetAllocator()),
       rapidjson::Value(), doc.GetAllocator());
      (*current)[key.c_str()].SetArray();
     }

     rapidjson::Value* the = &(*current)[key.c_str()];

     while (the->Size() <= static_cast<rapidjson::SizeType>(index)) {
      the->PushBack(rapidjson::Value(), doc.GetAllocator());
     }
     current = &(*current)[index];
    }
    else { // 对象部分处理
     if (!current->IsObject()) {
      current->SetObject();
     }
     if (!current->HasMember(part.c_str())) {
      current->AddMember(
       rapidjson::Value(part.c_str(), doc.GetAllocator()),
       rapidjson::Value(), doc.GetAllocator());
     }
     current = &(*current)[part.c_str()];
    }
   }

   // 根据类型设置值
   switch (node.second.Type()) {
   case Value::ValueType::String:
    current->SetString(node.second.Source().c_str(), doc.GetAllocator());
    break;
   case Value::ValueType::Int: {
    int32_t v = 0;
    memcpy(&v, node.second.Source().data(), sizeof(int32_t));
    current->SetInt(v);
    break;
   }
   case Value::ValueType::Uint: {
    uint32_t v = 0;
    memcpy(&v, node.second.Source().data(), sizeof(uint32_t));
    current->SetUint(v);
    break;
   }
   case Value::ValueType::Float: {
    float v = 0;
    memcpy(&v, node.second.Source().data(), sizeof(float));
    current->SetFloat(v);
    break;
   }
   default:
    current->SetNull();
    break;
   }
  }

  out = toString(doc);
 }

public:
 static std::string& Replace(std::string& str, const std::string& old_value,
  const std::string& new_value) {
  std::string::size_type pos(0);
  pos = 0;
  while (true) {
   if ((pos = str.find(old_value, pos)) != std::string::npos) {
    str.replace(pos, old_value.length(), new_value);
    pos = pos + new_value.length() + 1;
   }
   else
    break;
  }
  return str;
 }
#if 0
 static bool Sort(rapidjson::Value& inObj, rapidjson::Document& inDoc,
  const bool& level1 = false /*max_level<=1*/) {
  bool result = false;
  if (level1) {
   if (inObj.IsArray()) {
    do {
     if (inObj.Empty()) {
      result = true;
      break;
     }
     for (auto it = inObj.Begin(); it != inObj.End(); ++it) {
      if (it->IsArray() || it->IsObject())
       Sort(*it, inDoc, level1);
      else {
       std::vector<std::shared_ptr<rapidjson::Value>> sortCache;
       auto temp = std::make_shared<rapidjson::Value>(
        rapidjson::Value::GenericValue(it->GetType()));
       *temp = *it;
       sortCache.emplace_back(temp);
       std::sort(sortCache.begin(), sortCache.end(),
        [](auto& obj1, auto& obj2) -> bool {
         return (*obj1) < (*obj2);
        });
       inObj.Erase(inObj.Begin(), inObj.End());
       std::for_each(sortCache.begin(), sortCache.end(),
        [&](const auto& node) {
         inObj.PushBack(*node, inDoc.GetAllocator());
        });
      }
     }
     result = true;
    } while (0);
   }
   else if (inObj.IsObject()) {
    do {
     if (inObj.ObjectEmpty()) {
      result = true;
      break;
     }

     std::vector<std::shared_ptr<rapidjson::Value>> sortCache;
     std::map<std::shared_ptr<rapidjson::Value>,
      std::shared_ptr<rapidjson::Value>>
      sortSuccessCache;
     for (auto it = inObj.MemberBegin(); it != inObj.MemberEnd(); ++it) {
      auto tempName = std::make_shared<rapidjson::Value>(
       rapidjson::Value::GenericValue(it->name.GetType()));
      *tempName = it->name;
      auto tempValue = std::make_shared<rapidjson::Value>(
       rapidjson::Value::GenericValue(it->value.GetType()));
      *tempValue = it->value;
      sortSuccessCache.emplace(std::make_pair(tempName, tempValue));
      sortCache.emplace_back(tempName);
     }
     if (sortCache.empty())
      break;
     std::sort(sortCache.begin(), sortCache.end(),
      [&](auto& obj1, auto& obj2) -> bool {
       return (*obj1) < (*obj2);
      });
     inObj.RemoveAllMembers();
     std::for_each(sortCache.begin(), sortCache.end(),
      [&](const auto& node) {
       inObj.AddMember(*node, *sortSuccessCache[node],
        inDoc.GetAllocator());
      });
     result = true;
    } while (0);
   }
   return result;
  }

  if (inObj.IsArray()) {
   do {
    if (inObj.Empty()) {
     result = true;
     break;
    }
    std::vector<std::shared_ptr<rapidjson::Value>> sortCache;
    for (auto it = inObj.Begin(); it != inObj.End(); ++it) {
     if (it->IsArray() || it->IsObject()) {
      Sort(*it, inDoc);
     }
     else { //!@ Json array sort.
      auto temp = std::make_shared<rapidjson::Value>(
       rapidjson::Value::GenericValue(it->GetType()));
      *temp = *it;
      sortCache.emplace_back(temp);
     }
    }
    if (sortCache.empty()) {
     result = true;
     break;
    }
    std::sort(
     sortCache.begin(), sortCache.end(),
     [](auto& obj1, auto& obj2) -> bool { return (*obj1) < (*obj2); });
    inObj.Erase(inObj.Begin(), inObj.End());
    std::for_each(sortCache.begin(), sortCache.end(),
     [&](const auto& node) {
      inObj.PushBack(*node, inDoc.GetAllocator());
     });
    result = true;
   } while (0);
  }
  else if (inObj.IsObject()) {
   do {
    if (inObj.ObjectEmpty()) {
     result = true;
     break;
    }
    std::vector<std::shared_ptr<rapidjson::Value>> sortCache;
    std::map<std::shared_ptr<rapidjson::Value>,
     std::shared_ptr<rapidjson::Value>>
     sortSuccessCache;
    for (auto it = inObj.MemberBegin(); it != inObj.MemberEnd(); ++it) {
     auto tempName = std::make_shared<rapidjson::Value>(
      rapidjson::Value::GenericValue(it->name.GetType()));
     *tempName = it->name;
     auto tempValue = std::make_shared<rapidjson::Value>(
      rapidjson::Value::GenericValue(it->value.GetType()));
     *tempValue = it->value;
     sortSuccessCache.emplace(std::make_pair(tempName, tempValue));
     sortCache.emplace_back(tempName);
    }
    if (sortCache.empty()) {
     result = true;
     break;
    }
    std::sort(
     sortCache.begin(), sortCache.end(),
     [&](auto& obj1, auto& obj2) -> bool { return (*obj1) < (*obj2); });
    inObj.RemoveAllMembers();
    std::for_each(sortCache.begin(), sortCache.end(),
     [&](const auto& node) {
      inObj.AddMember(*node, *sortSuccessCache[node],
       inDoc.GetAllocator());
     });
    for (auto it = inObj.MemberBegin(); it != inObj.MemberEnd(); ++it) {
     if (it->value.IsArray() || it->value.IsObject())
      Sort(it->value, inDoc);
    }
    result = true;
   } while (0);
  }
  return true;
 }
#endif
 static std::string toString(const rapidjson::Value& valObj) {
  std::string result;
  rapidjson::StringBuffer jbuffer;
  rapidjson::Writer<rapidjson::StringBuffer> jwriter(jbuffer);
  if (valObj.Accept(jwriter))
   result = std::string(jbuffer.GetString(), jbuffer.GetLength());
  return result;
 }
 static void Compress(rapidjson::Value& targetObj, rapidjson::Value& host) {
  do {
   if (!targetObj.IsObject())
    break;
   if (targetObj.ObjectEmpty())
    break;
   for (auto itObj = targetObj.MemberBegin();
    itObj != targetObj.MemberEnd();) {
    auto name = itObj->name.GetString();
    if (itObj->value.IsArray()) {
     rapidjson::SizeType aysize = itObj->value.Size();
     if (aysize <= 0) {
      itObj = targetObj.EraseMember(itObj);
      continue;
     }
     for (rapidjson::SizeType i = 0; i < aysize; ++i) {
      if (itObj->value[i].IsObject()) {
       if (itObj->value[i].IsNull() || itObj->value[i].ObjectEmpty()) {
        itObj->value.RemoveMember(itObj->value[i]);
        continue;
       }
       else
        Compress(itObj->value[i], targetObj);
      }
     }
    }
    else if (itObj->value.IsObject()) {
     if (itObj->value.ObjectEmpty() || itObj->value.IsNull()) {
      itObj = targetObj.EraseMember(itObj);
      continue;
     }
     else
      Compress(itObj->value, targetObj);
    }
    ++itObj;
   }
   if (targetObj.ObjectEmpty())
    Compress(host, targetObj);
  } while (0);
 }

 static std::string Format(const std::string& jsonPak,
  const std::string& levelStr = "\t") {
  std::string result;
  if (jsonPak.empty())
   return "";
  int level = 0;
  for (std::string::size_type index = 0; index < jsonPak.size(); index++) {
   char c = jsonPak[index];
   if (level > 0 && '\n' == *std::prev(jsonPak.end())) {
    result += levelStr;
   }
   switch (c) {
   case '{':
   case '[':
    result = result + c + "\n";
    level++;
    result += levelStr;
    break;
   case ',':
    result = result + c + "\n";
    result += levelStr;
    break;
   case '}':
   case ']':
    result += "\n";
    level--;
    result += levelStr;
    result += c;
    break;
   default:
    result += c;
    break;
   }
  }
  return result;
 }

 static void Enumerate(const rapidjson::Value& value, int depth = 0) {
  if (value.IsObject()) {
   for (auto itr = value.MemberBegin(); itr != value.MemberEnd(); ++itr) {
    std::cout << std::string(depth, ' ') << "Key: " << itr->name.GetString()
     << "\n";
    Enumerate(itr->value, depth + 1);
   }
  }
  else if (value.IsArray()) {
   for (rapidjson::SizeType i = 0; i < value.Size(); ++i) {
    std::cout << std::string(depth, ' ') << "Index: " << i << "\n";
    Enumerate(value[i], depth + 1);
   }
  }
  else if (value.IsString()) {
   std::cout << std::string(depth, ' ') << "String: " << value.GetString()
    << "\n";
  }
  else if (value.IsInt()) {
   std::cout << std::string(depth, ' ') << "Int: " << value.GetInt() << "\n";
  }
  else if (value.IsBool()) {
   std::cout << std::string(depth, ' ')
    << "Bool: " << (value.GetBool() ? "true" : "false") << "\n";
  }
  else if (value.IsNull()) {
   std::cout << std::string(depth, ' ') << "Null\n";
  }
 }

public:
 static void Sort(rapidjson::Value& value,
  rapidjson::Document::AllocatorType& allocator) {
  if (value.IsObject()) {
   std::vector<std::pair<std::string, rapidjson::Value>> members;
   for (auto itr = value.MemberBegin(); itr != value.MemberEnd(); ++itr) {
    rapidjson::Value& v = itr->value;
    Sort(v, allocator);
    if ((v.IsObject() && v.ObjectEmpty()) || (v.IsArray() && v.Empty())) {
     continue;
    }
    members.emplace_back(itr->name.GetString(),
     rapidjson::Value(v, allocator));
   }
   std::sort(members.begin(), members.end(),
    [](const auto& a, const auto& b) { return a.first < b.first; });

   value.RemoveAllMembers();
   for (auto& m : members) {
    rapidjson::Value name(m.first.c_str(), allocator);
    value.AddMember(name, m.second, allocator);
   }
  }
  else if (value.IsArray()) {
   rapidjson::Value newArray(rapidjson::kArrayType);
   for (auto& v : value.GetArray()) {
    Sort(v, allocator);
    if ((v.IsObject() && v.ObjectEmpty()) || (v.IsArray() && v.Empty())) {
     continue;
    }
    newArray.PushBack(v, allocator);
   }
   value = std::move(newArray);
  }
 }
};
/// /*_ Memade®（新生™） _**/
/// /*_ Wed, 30 Oct 2024 05:51:18 GMT _**/
/// /*_____ https://www.skstu.com/ _____ **/
#endif///__BE568688_DF41_4AE6_8154_49C6B7EEDDF4__
