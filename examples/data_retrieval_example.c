#include "bitcoinkernel.h"

#include <stdio.h>
#include <stdlib.h>

void handle_error(void* _, const char* msg, size_t msg_size)
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
    kernel_ChainParameters* chainparams = kernel_chain_parameters_create(kernel_CHAIN_TYPE_REGTEST);
    kernel_context_options_set_chainparams(context_options, chainparams);
    kernel_chain_parameters_destroy(chainparams);
    kernel_Context* context = kernel_context_create(context_options);
    kernel_context_options_destroy(context_options);
    if (context == NULL) return 1;

    const char data_dir[] = ".bitcoin";
    const char blocks_dir[] = ".bitcoin/blocks";
    kernel_ChainstateManagerOptions* chainman_options = kernel_chainstate_manager_options_create(context, data_dir, sizeof(data_dir) - 1, blocks_dir, sizeof(blocks_dir));
    if (chainman_options == NULL) return 1;
    kernel_ChainstateManager* chainman = kernel_chainstate_manager_create(context, chainman_options);
    if (chainman == NULL) return 1;
    kernel_chainstate_manager_options_destroy(chainman_options);

    kernel_BlockIndex* index = kernel_get_block_index_from_genesis(context, chainman);
    kernel_Block* genesis = kernel_read_block_from_disk(context, chainman, index);
    // Now do something with this genesis block.

    kernel_block_destroy(genesis);
    kernel_chainstate_manager_destroy(chainman, context);
    kernel_context_destroy(context);

    return 0;
}
