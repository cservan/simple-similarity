PROJECT(topicAdaptation)


include_directories(BEFORE ${CMAKE_SOURCE_DIR}/src)
# include_directories(BEFORE )

# SET(filteringOutputs_SRCS
#    main.cpp ${CMAKE_SOURCE_DIR}/src/tools.cpp ${CMAKE_SOURCE_DIR}/src/hashMap.cpp ${CMAKE_SOURCE_DIR}/src/stringHasher.cpp
# )

# SET(extractDictionnary_SRCS
#    main.cin.cpp ${CMAKE_SOURCE_DIR}/src/tools.cpp ${CMAKE_SOURCE_DIR}/src/hashMap.cpp ${CMAKE_SOURCE_DIR}/src/stringHasher.cpp
# )

#SET(tfidf_SRCS
#    main.detect.cpp ${CMAKE_SOURCE_DIR}/src/tools.cpp ${CMAKE_SOURCE_DIR}/src/hashMap.cpp ${CMAKE_SOURCE_DIR}/src/stringHasher.cpp ${CMAKE_SOURCE_DIR}/src/hashMapStringInfos.cpp ${CMAKE_SOURCE_DIR}/src/stringInfosHasher.cpp  ${CMAKE_SOURCE_DIR}/src/srilm/File.cc ${CMAKE_SOURCE_DIR}/src/srilm/LHash.cc ${CMAKE_SOURCE_DIR}/src/srilm/Map.cc ${CMAKE_SOURCE_DIR}/src/srilm/Vocab.cc ${CMAKE_SOURCE_DIR}/src/srilm/zio.c
#   main.tfidf.cpp ${CMAKE_SOURCE_DIR}/src/tools.cpp ${CMAKE_SOURCE_DIR}/src/tfidf.cpp 
#)
SET(similarity_SRCS
   main.similarity.cpp ${CMAKE_SOURCE_DIR}/src/tools.cpp ${CMAKE_SOURCE_DIR}/src/similarity.cpp ${CMAKE_SOURCE_DIR}/src/myIndex.cpp ${CMAKE_SOURCE_DIR}/src/tfidf.cpp 
)

#SET(similarityBySentence_SRCS
#   main.similarityBySentence.cpp ${CMAKE_SOURCE_DIR}/src/tools.cpp ${CMAKE_SOURCE_DIR}/src/similarity.cpp ${CMAKE_SOURCE_DIR}/src/tfidf.cpp 
#)

#SET(filterPhraseTable_SRCS
#  main.filterPhraseTable.cpp ${CMAKE_SOURCE_DIR}/src/tools.cpp ${CMAKE_SOURCE_DIR}/src/phrase.cpp ${CMAKE_SOURCE_DIR}/src/ptable.cpp
#)
#SET(addTranslations_SRCS
#  main.addTranslations.cpp ${CMAKE_SOURCE_DIR}/src/tools.cpp ${CMAKE_SOURCE_DIR}/src/phrase.cpp ${CMAKE_SOURCE_DIR}/src/ptable.cpp
#)
#SET(extractTranslations_SRCS
#  main.extractTranslations.cpp ${CMAKE_SOURCE_DIR}/src/tools.cpp ${CMAKE_SOURCE_DIR}/src/phrase.cpp ${CMAKE_SOURCE_DIR}/src/ptable.cpp ${CMAKE_SOURCE_DIR}/src/similarity.cpp ${CMAKE_SOURCE_DIR}/src/tfidf.cpp 
#)
#SET(extractTranslationsFromAlignments_SRCS
#  main.extractTranslationsFromAlignments.cpp ${CMAKE_SOURCE_DIR}/src/tools.cpp ${CMAKE_SOURCE_DIR}/src/phrase.cpp ${CMAKE_SOURCE_DIR}/src/ptable.cpp ${CMAKE_SOURCE_DIR}/src/similarity.cpp ${CMAKE_SOURCE_DIR}/src/tfidf.cpp 
#)
# SET(extractAndDetectOOV_SRCS
#    main.extract_and_detect.cpp ${CMAKE_SOURCE_DIR}/src/tools.cpp ${CMAKE_SOURCE_DIR}/src/hashMap.cpp ${CMAKE_SOURCE_DIR}/src/stringHasher.cpp ${CMAKE_SOURCE_DIR}/src/hashMapStringInfos.cpp ${CMAKE_SOURCE_DIR}/src/stringInfosHasher.cpp  
# )

# add_executable(filteringOutputs ${filteringOutputs_SRCS})
# add_executable(extractDictionnary ${extractDictionnary_SRCS})
#add_executable(tfidf.0.1 ${tfidf_SRCS})
#target_link_libraries(tfidf.0.1  boost_regex boost_system boost_filesystem)

add_executable(similarity.0.5 ${similarity_SRCS})
#add_executable(similarityBySentence.0.1 ${similarityBySentence_SRCS})
#add_executable(filterPhraseTable.0.1 ${filterPhraseTable_SRCS})
#add_executable(addTranslations.0.1 ${addTranslations_SRCS})
#add_executable(extractTranslations.0.1 ${extractTranslations_SRCS})
#add_executable(extractTranslationsFromAlignments.0.1 ${extractTranslationsFromAlignments_SRCS})
#target_link_libraries(similarityBySentence.0.1  boost_regex boost_system boost_filesystem boost_iostreams)
target_link_libraries(similarity.0.5  boost_regex boost_system boost_filesystem boost_iostreams gomp boost_thread)
#target_link_libraries(addTranslations.0.1  boost_regex boost_system boost_filesystem boost_iostreams)
#target_link_libraries(filterPhraseTable.0.1 boost_regex boost_system boost_filesystem boost_iostreams)
#target_link_libraries(extractTranslations.0.1  boost_regex boost_system boost_filesystem boost_iostreams)
#target_link_libraries(extractTranslationsFromAlignments.0.1 boost_regex boost_system boost_filesystem boost_iostreams)
# target_link_libraries(lemmatize.0.1)
# target_link_libraries(extractAndDetectOOV  boost_regex)

# install(TARGETS extractAndDetectOOV detectWords.0.5 filteringOutputs extractDictionnary DESTINATION bin)
install(TARGETS similarity.0.5 DESTINATION bin)


