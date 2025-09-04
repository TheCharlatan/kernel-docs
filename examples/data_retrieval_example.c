#include "bitcoinkernel.h"

#include <stdio.h>
#include <stdlib.h>

void handle_error(void* _, const char* msg, size_t msg_size)
{
    printf("%s\n", msg);
    exit(0);
}

int main() {
    btck_ContextOptions* context_options = btck_context_options_create();
    btck_NotificationInterfaceCallbacks notifications = {
        .user_data = NULL,
        .block_tip = NULL,
        .header_tip = NULL,
        .progress = NULL,
        .warning_set = NULL,
        .warning_unset = NULL,
        .flush_error = handle_error,
        .fatal_error = handle_error,
    };
    btck_context_options_set_notifications(context_options, notifications);
    btck_ChainParameters* chainparams = btck_chain_parameters_create(btck_ChainType_REGTEST);
    btck_context_options_set_chainparams(context_options, chainparams);
    btck_chain_parameters_destroy(chainparams);
    btck_Context* context = btck_context_create(context_options);
    btck_context_options_destroy(context_options);
    if (context == NULL) return 1;

    const char data_dir[] = ".bitcoin";
    const char blocks_dir[] = ".bitcoin/blocks";
    btck_ChainstateManagerOptions* chainman_options = btck_chainstate_manager_options_create(context, data_dir, sizeof(data_dir) - 1, blocks_dir, sizeof(blocks_dir));
    if (chainman_options == NULL) return 1;
    btck_ChainstateManager* chainman = btck_chainstate_manager_create(chainman_options);
    if (chainman == NULL) return 1;
    btck_chainstate_manager_options_destroy(chainman_options);

    const btck_Chain* chain = btck_chainstate_manager_get_active_chain(chainman);
    btck_BlockTreeEntry* entry = btck_chain_get_genesis(chain);
    btck_Block* genesis = btck_block_read(chainman, entry);
    // Now do something with this genesis block.

    btck_block_destroy(genesis);
    btck_chainstate_manager_destroy(chainman);
    btck_context_destroy(context);

    return 0;
}
