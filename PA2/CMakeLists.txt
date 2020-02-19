cmake_minimum_required (VERSION 2.6)
project (spam)
add_executable(training src/training.cc)
target_compile_features(training PRIVATE cxx_range_for)

add_executable(classify src/classify.cc)
target_compile_features(classify PRIVATE cxx_range_for)

