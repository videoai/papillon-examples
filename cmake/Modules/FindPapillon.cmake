

find_path($ENV{PAPILLON_INSTALL_DIR} NAMES include/PapillonCore/PapillonCore.h)

find_library(PAPILLON_LIBRARIES NAMES PapillonCore PATHS $ENV{PAPILLON_INSTALL_DIR}/lib NO_DEFAULT_PATH)

set(PAPILLON_INCLUDE_DIR $ENV{PAPILLON_INSTALL_DIR}/include/PapillonCore)

find_package_handle_standard_args(Papillon FOUND_VAR Papillon_FOUND REQUIRED_VARS PAPILLON_LIBRARIES PAPILLON_INCLUDE_DIR)