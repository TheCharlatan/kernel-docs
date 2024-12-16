#include "bitcoinkernel.h"

#include <stdio.h>
#include <stdlib.h>

void handle_error(void* _, const char* msg)
{
    printf("%s\n", msg);
    exit(0);
}

int main() {
    kernel_ContextOptions* context_options = kernel_context_options_create();
    kernel_NotificationInterfaceCallbacks notifications = {
        .user_data = NULL,
        .block_tip = NULL,
        .header_tip = NULL,
        .progress = NULL,
        .warning_set = NULL,
        .warning_unset = NULL,
        .flush_error = handle_error,
        .fatal_error = handle_error,
    };
    kernel_context_options_set_notifications(context_options, notifications);
    const kernel_ChainParameters* chainparams = kernel_chain_parameters_create(kernel_CHAIN_TYPE_REGTEST);
    kernel_context_options_set_chainparams(context_options, chainparams);
    kernel_chain_parameters_destroy(chainparams);
    kernel_Context* context = kernel_context_create(context_options);
    kernel_context_options_destroy(context_options);
    if (context == NULL) return 1;

    kernel_ChainstateManagerOptions* chainman_options = kernel_chainstate_manager_options_create(context, ".bitcoin");
    if (chainman_options == NULL) return 1;
    kernel_BlockManagerOptions* blockman_options = kernel_block_manager_options_create(context, ".bitcoin/blocks");
    if (blockman_options == NULL) return 1;
    kernel_ChainstateManager* chainman = kernel_chainstate_manager_create(context, chainman_options, blockman_options);
    if (chainman == NULL) return 1;
    kernel_chainstate_manager_options_destroy(chainman_options);
    kernel_block_manager_options_destroy(blockman_options);

    kernel_ChainstateLoadOptions* chainstate_load_options = kernel_chainstate_load_options_create();
    if (!kernel_chainstate_manager_load_chainstate(context, chainstate_load_options, chainman)) {
        return 1;
    }
    kernel_chainstate_load_options_destroy(chainstate_load_options);

    kernel_BlockIndex* index = kernel_get_block_index_from_genesis(context, chainman);
    kernel_Block* genesis = kernel_read_block_from_disk(context, chainman, index);
    // Now do something with this genesis block.

    kernel_block_destroy(genesis);
    kernel_chainstate_manager_destroy(chainman, context);
    kernel_context_destroy(context);

    return 0;
}
