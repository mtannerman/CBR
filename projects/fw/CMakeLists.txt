PROJECT(fw)

SET(FW_INC
    inc/fw/logging.h)

SET(FW_SRC
    src/logging.cpp)
    
SET(UTF8_INC
	inc/utf8/core.h	
	inc/utf8/utf8.h
	inc/utf8/u8.h
	)

SET(UTF8_SRC	
    )
    
SET(CEREAL_INC
	inc/cereal/access.hpp
	inc/cereal/cereal.hpp
	inc/cereal/macros.hpp
	)
	
SET(CEREAL_ARCHIVES_INC
	inc/cereal/archives/adapters.hpp
	inc/cereal/archives/binary.hpp
	inc/cereal/archives/json.hpp
	inc/cereal/archives/portable_binary.hpp
	inc/cereal/archives/xml.hpp
	)	

SET(CEREAL_DETAILS_INC
	inc/cereal/details/helpers.hpp
	inc/cereal/details/polymorphic_impl.hpp
	inc/cereal/details/static_object.hpp
	inc/cereal/details/traits.hpp
	inc/cereal/details/util.hpp
	)
	
SET(CEREAL_TYPES_INC	
	inc/cereal/types/array.hpp
	inc/cereal/types/base_class.hpp
	inc/cereal/types/bitset.hpp
	inc/cereal/types/boost_variant.hpp
	inc/cereal/types/chrono.hpp
	inc/cereal/types/common.hpp
	inc/cereal/types/complex.hpp
	inc/cereal/types/deque.hpp
	inc/cereal/types/forward_list.hpp
	inc/cereal/types/list.hpp
	inc/cereal/types/map.hpp
	inc/cereal/types/memory.hpp
	inc/cereal/types/optional.hpp
	inc/cereal/types/polymorphic.hpp
	inc/cereal/types/queue.hpp
	inc/cereal/types/set.hpp
	inc/cereal/types/stack.hpp
	inc/cereal/types/string.hpp
	inc/cereal/types/tuple.hpp
	inc/cereal/types/unordered_map.hpp
	inc/cereal/types/unordered_set.hpp
	inc/cereal/types/utility.hpp
	inc/cereal/types/valarray.hpp
	inc/cereal/types/vector.hpp	
	)
	
SET(BASE64_INC
	inc/base64/base64.hpp
	)

SET(RAPID_XML_INC
	inc/rapidxml/rapidxml.hpp
	inc/rapidxml/rapidxml_iterators.hpp
	inc/rapidxml/rapidxml_print.hpp
	inc/rapidxml/rapidxml_utils.hpp
	)

SET(RAPID_JSON_INC
	inc/rapidjson/allocators.h
	inc/rapidjson/document.h
	inc/rapidjson/encodedstream.h
	inc/rapidjson/encodings.h
	inc/rapidjson/filereadstream.h
	inc/rapidjson/filewritestream.h
	inc/rapidjson/fwd.h
	inc/rapidjson/istreamwrapper.h
	inc/rapidjson/memorybuffer.h
	inc/rapidjson/memorystream.h
	inc/rapidjson/ostreamwrapper.h
	inc/rapidjson/pointer.h
	inc/rapidjson/prettywriter.h
	inc/rapidjson/rapidjson.h
	inc/rapidjson/reader.h
	inc/rapidjson/schema.h
	inc/rapidjson/stream.h
	inc/rapidjson/stringbuffer.h
	inc/rapidjson/writer.h
	)

SET(RAPID_JSON_INTERNAL_INC
	inc/rapidjson/internal/biginteger.h
	inc/rapidjson/internal/diyfp.h
	inc/rapidjson/internal/dtoa.h
	inc/rapidjson/internal/ieee754.h
	inc/rapidjson/internal/itoa.h
	inc/rapidjson/internal/meta.h
	inc/rapidjson/internal/pow10.h
	inc/rapidjson/internal/regex.h
	inc/rapidjson/internal/stack.h
	inc/rapidjson/internal/strfunc.h
	inc/rapidjson/internal/strtod.h
	inc/rapidjson/internal/swap.h		
    )
    
SET(OPTIONAL_INC
	inc/optional/optional.hpp
	)

SOURCE_GROUP("fw" FILES ${FW_INC} ${FW_SRC})
SOURCE_GROUP("utf8" FILES ${UTF8_INC} ${UTF8_SRC})
SOURCE_GROUP("optional" FILES ${OPTIONAL_INC})
SOURCE_GROUP("cereal" FILES ${CEREAL_INC})
SOURCE_GROUP("cereal\\archives" FILES ${CEREAL_ARCHIVES_INC})
SOURCE_GROUP("cereal\\details" FILES ${CEREAL_DETAILS_INC})
SOURCE_GROUP("cereal\\types" FILES ${CEREAL_TYPES_INC})
SOURCE_GROUP("base64" FILES ${BASE64_INC})
SOURCE_GROUP("rapidxml" FILES ${RAPID_XML_INC})
SOURCE_GROUP("rapidjson" FILES ${RAPID_JSON_INC})
SOURCE_GROUP("rapidjson\\internal" FILES ${RAPID_JSON_INTERNAL_INC})

INCLUDE_DIRECTORIES(inc/)

ADD_LIBRARY(fw SHARED
${FW_INC} ${FW_SRC}
${UTF8_INC} ${UTF8_SRC}
${CEREAL_INC}
${CEREAL_ARCHIVES_INC}
${CEREAL_DETAILS_INC}
${CEREAL_TYPES_INC}
${BASE64_INC}
${RAPID_XML_INC}
${RAPID_JSON_INC}
${RAPID_JSON_INTERNAL_INC}
${OPTIONAL_INC}
)