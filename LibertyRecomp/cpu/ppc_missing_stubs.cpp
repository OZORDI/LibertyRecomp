/*
 * Stub implementations for PPC functions that are declared via PPC_EXTERN_IMPORT
 * in ppc_func_decls.h and referenced by hooks, but NOT present in the codegen
 * output (gta4_recomp.*.cpp). Without the original PPC binary decompilation for
 * these addresses, we provide no-op stubs so the link succeeds. Hooks that call
 * through to these originals will effectively skip the original behavior.
 *
 * If the codegen is re-run and produces implementations for any of these, remove
 * the corresponding stub from this file.
 */

#include <rex/ppc/context.h>

// game_init.cpp
PPC_FUNC_IMPL(__imp__sub_8218C600) { }
PPC_FUNC_IMPL(__imp__sub_82120EE8) { }
PPC_FUNC_IMPL(__imp__sub_821250B0) { }
PPC_FUNC_IMPL(__imp__sub_82124080) { }
PPC_FUNC_IMPL(__imp__sub_82120FB8) { }
PPC_FUNC_IMPL(__imp__sub_82318F60) { }

// save_hooks.cpp
PPC_FUNC_IMPL(__imp__sub_821200D0) { }

// menu_hooks.cpp
PPC_FUNC_IMPL(__imp__sub_8212FCC8) { }
PPC_FUNC_IMPL(__imp__sub_821485D8) { }
PPC_FUNC_IMPL(__imp__sub_82273620) { }

// imports.cpp
PPC_FUNC_IMPL(__imp__sub_828507F8) { }
PPC_FUNC_IMPL(__imp__sub_8218BE28) { }

// audio_patches.cpp
PPC_FUNC_IMPL(__imp__sub_822EEDB8) { }
PPC_FUNC_IMPL(__imp__sub_821AB5F8) { }

// gta4_motion_patches.cpp
PPC_FUNC_IMPL(__imp__sub_82593668) { }
PPC_FUNC_IMPL(__imp__sub_82590628) { }
PPC_FUNC_IMPL(__imp__sub_82590918) { }
PPC_FUNC_IMPL(__imp__sub_82587310) { }
PPC_FUNC_IMPL(__imp__sub_82579960) { }

// player_limit_patches.cpp
PPC_FUNC_IMPL(__imp__sub_8238F700) { }
PPC_FUNC_IMPL(__imp__sub_8267D798) { }
PPC_FUNC_IMPL(__imp__sub_8267D910) { }
PPC_FUNC_IMPL(__imp__sub_8267DB90) { }
PPC_FUNC_IMPL(__imp__sub_8267DE28) { }
PPC_FUNC_IMPL(__imp__sub_8267E198) { }
PPC_FUNC_IMPL(__imp__sub_8267EBA8) { }
PPC_FUNC_IMPL(__imp__sub_8267ECF0) { }
PPC_FUNC_IMPL(__imp__sub_8267F838) { }
PPC_FUNC_IMPL(__imp__sub_82683998) { }
PPC_FUNC_IMPL(__imp__sub_82685EB0) { }
PPC_FUNC_IMPL(__imp__sub_826861D0) { }
PPC_FUNC_IMPL(__imp__sub_82689828) { }
PPC_FUNC_IMPL(__imp__sub_82689998) { }
PPC_FUNC_IMPL(__imp__sub_826AE960) { }
PPC_FUNC_IMPL(__imp__sub_826AF5B8) { }
PPC_FUNC_IMPL(__imp__sub_826AF748) { }
PPC_FUNC_IMPL(__imp__sub_826B0720) { }
PPC_FUNC_IMPL(__imp__sub_826B0830) { }
PPC_FUNC_IMPL(__imp__sub_827FC738) { }

// text_patches.cpp
PPC_FUNC_IMPL(__imp__sub_825ECB48) { }
PPC_FUNC_IMPL(__imp__sub_821735B8) { }
PPC_FUNC_IMPL(__imp__sub_82173838) { }

// video_patches.cpp
PPC_FUNC_IMPL(__imp__sub_826078D8) { }
PPC_FUNC_IMPL(__imp__sub_82619D00) { }
PPC_FUNC_IMPL(__imp__sub_82619B88) { }
PPC_FUNC_IMPL(__imp__sub_82619FF0) { }
