#include "bitcoinkernel.h"

#include "stddef.h"

int main() {
    // Taproot transaction 33e794d097969002ee05d336686fc03c9e15a597c1b9827669460fac98799036
    static const unsigned char tx[] = {
       0x01, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0xd1, 0xf1, 0xc1, 0xf8, 0xcd, 0xf6, 0x75, 0x91, 0x67,
       0xb9, 0x0f, 0x52, 0xc9, 0xad, 0x35, 0x8a, 0x36, 0x9f, 0x95, 0x28, 0x4e, 0x84, 0x1d, 0x7a, 0x25,
       0x36, 0xce, 0xf3, 0x1c, 0x05, 0x49, 0x58, 0x01, 0x00, 0x00, 0x00, 0x00, 0xfd, 0xff, 0xff, 0xff,
       0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x31, 0x6a, 0x2f, 0x49, 0x20, 0x6c, 0x69,
       0x6b, 0x65, 0x20, 0x53, 0x63, 0x68, 0x6e, 0x6f, 0x72, 0x72, 0x20, 0x73, 0x69, 0x67, 0x73, 0x20,
       0x61, 0x6e, 0x64, 0x20, 0x49, 0x20, 0x63, 0x61, 0x6e, 0x6e, 0x6f, 0x74, 0x20, 0x6c, 0x69, 0x65,
       0x2e, 0x20, 0x40, 0x62, 0x69, 0x74, 0x62, 0x75, 0x67, 0x34, 0x32, 0x9e, 0x06, 0x01, 0x00, 0x00,
       0x00, 0x00, 0x00, 0x22, 0x51, 0x20, 0xa3, 0x7c, 0x39, 0x03, 0xc8, 0xd0, 0xdb, 0x65, 0x12, 0xe2,
       0xb4, 0x0b, 0x0d, 0xff, 0xa0, 0x5e, 0x5a, 0x3a, 0xb7, 0x36, 0x03, 0xce, 0x8c, 0x9c, 0x4b, 0x77,
       0x71, 0xe5, 0x41, 0x23, 0x28, 0xf9, 0x01, 0x40, 0xa6, 0x0c, 0x38, 0x3f, 0x71, 0xba, 0xc0, 0xec,
       0x91, 0x9b, 0x1d, 0x7d, 0xbc, 0x3e, 0xb7, 0x2d, 0xd5, 0x6e, 0x7a, 0xa9, 0x95, 0x83, 0x61, 0x55,
       0x64, 0xf9, 0xf9, 0x9b, 0x8a, 0xe4, 0xe8, 0x37, 0xb7, 0x58, 0x77, 0x3a, 0x5b, 0x2e, 0x4c, 0x51,
       0x34, 0x88, 0x54, 0xc8, 0x38, 0x9f, 0x00, 0x8e, 0x05, 0x02, 0x9d, 0xb7, 0xf4, 0x64, 0xa5, 0xff,
       0x2e, 0x01, 0xd5, 0xe6, 0xe6, 0x26, 0x17, 0x4a, 0xff, 0xd3, 0x0a, 0x00
    };

    // Spends script pubkey 5120339ce7e165e67d93adb3fef88a6d4beed33f01fa876f05a225242b82a631abc0
    static const unsigned char script[] = {
        0x51, 0x20, 0x33, 0x9c, 0xe7, 0xe1, 0x65, 0xe6, 0x7d, 0x93, 0xad, 0xb3, 0xfe, 0xf8, 0x8a, 0x6d,
        0x4b, 0xee, 0xd3, 0x3f, 0x01, 0xfa, 0x87, 0x6f, 0x05, 0xa2, 0x25, 0x24, 0x2b, 0x82, 0xa6, 0x31,
        0xab, 0xc0
    }; 

    kernel_Transaction* transaction = kernel_transaction_create(tx, sizeof(tx));
    if (transaction == NULL) return 1;
    kernel_ScriptPubkey* script_pubkey = kernel_script_pubkey_create(script, sizeof(script));
    if (script_pubkey == NULL) return 1;
    int64_t amount = 88480;

    kernel_TransactionOutput* output = kernel_transaction_output_create(script_pubkey, amount);
    const kernel_TransactionOutput* output_ = output;

    kernel_ScriptVerifyStatus status = kernel_SCRIPT_VERIFY_OK;

    bool result = kernel_verify_script(
            /*script_pubkey=*/ script_pubkey,
            /*amount=*/ amount,
            /*tx_to=*/ transaction,
            /*spent_outputs=*/ &output_,
            /*spent_outputs_len=*/1,
            /*input_index=*/ 0,
            /*flags=*/ kernel_SCRIPT_FLAGS_VERIFY_ALL,
            /*status*/ &status);

    kernel_transaction_output_destroy(output);
    kernel_script_pubkey_destroy(script_pubkey);
    kernel_transaction_destroy(transaction);

    return !(result && status == kernel_SCRIPT_VERIFY_OK);
}