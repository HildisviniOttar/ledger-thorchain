/*******************************************************************************
*   (c) 2016 Ledger
*   (c) 2018 ZondaX GmbH
*
*  Licensed under the Apache License, Version 2.0 (the "License");
*  you may not use this file except in compliance with the License.
*  You may obtain a copy of the License at
*
*      http://www.apache.org/licenses/LICENSE-2.0
*
*  Unless required by applicable law or agreed to in writing, software
*  distributed under the License is distributed on an "AS IS" BASIS,
*  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*  See the License for the specific language governing permissions and
*  limitations under the License.
********************************************************************************/
#include "signature.h"

#include "cx.h"
#include "apdu_codes.h"

// TODO: This is only temporary and is using ETH derivation path = 44'/60'/0'/0/0
const uint32_t bip32_derivation_path[] =
{
        0x80000000 | 44,
        0x80000000 | 60,
        0x80000000 | 0,
        0x80000000 | 0,
        0x80000000 | 0
};

void generate_public_key(
        uint8_t* public_key,
        unsigned int public_key_capacity,
        unsigned int* public_key_length)
{
    // Generate keys
    uint8_t privateKeyData[32];
    os_perso_derive_node_bip32(
            CX_CURVE_256K1,
            bip32_derivation_path,
            sizeof(bip32_derivation_path) / sizeof(uint32_t),
            privateKeyData,
            NULL);

    cx_ecfp_private_key_t privateKey;
    cx_ecfp_init_private_key(
            CX_CURVE_256K1,
            privateKeyData,
            32,
            &privateKey);

    cx_ecfp_public_key_t publicKey;
    cx_ecfp_generate_pair(CX_CURVE_256K1,
                          &publicKey,
                          &privateKey,
                          1);

    if (public_key_capacity < sizeof(publicKey.W) || public_key_length == NULL)
    {
        THROW(APDU_CODE_OUTPUT_BUFFER_TOO_SMALL);
    }
    os_memmove(public_key, publicKey.W, sizeof(publicKey.W));
    *public_key_length = sizeof(publicKey.W);
}

int sign_secp256k1(
        const uint8_t* message,
        unsigned int message_length,
        uint8_t* signature,
        unsigned int signature_capacity,
        unsigned int* signature_length)
{
    uint8_t message_digest[CX_SHA256_SIZE];
    cx_hash_sha256(message, message_length, message_digest, CX_SHA256_SIZE);

    // Generate keys
    uint8_t privateKeyData[32];
    os_perso_derive_node_bip32(
        CX_CURVE_256K1,
        bip32_derivation_path,
        sizeof(bip32_derivation_path) / sizeof(uint32_t),
        privateKeyData,
        NULL);

    cx_ecfp_private_key_t privateKey;
    cx_ecfp_init_private_key(
        CX_CURVE_256K1,
        privateKeyData,
        32,
        &privateKey);

    cx_ecfp_public_key_t publicKey;
    cx_ecfp_generate_pair(CX_CURVE_256K1,
                          &publicKey,
                          &privateKey,
                          1);

    unsigned int info = 0;

    *signature_length = cx_ecdsa_sign(
            &privateKey,
            CX_RND_RFC6979 | CX_LAST,
            CX_SHA256,
            message_digest,
            sizeof(message_digest),
            signature,
            signature_capacity,
            &info);

    if (info & CX_ECCINFO_PARITY_ODD) {
        signature[0] |= 0x01;
    }

    os_memset(&privateKey, 0, sizeof(privateKey));

    return cx_ecdsa_verify(
            &publicKey,
            CX_LAST,
            CX_SHA256,
            message_digest,
            sizeof(message_digest),
            (unsigned char *) signature,
            *signature_length);
}

#ifdef ed25519
int generate_signature_ed25519(uint8_t *message, uint16_t message_length)
{
    uint8_t message_digest[CX_SHA512_SIZE];
    cx_hash_sha512(message, message_length, message_digest, CX_SHA512_SIZE);

    // Reset signature
    os_memset((void *) signature, 0, sizeof(signature));
    length = 0;

    uint8_t privateKeyData[32];
    cx_ecfp_private_key_t privateKey;
    cx_ecfp_public_key_t publicKey;

    os_perso_derive_node_bip32(
        CX_CURVE_Ed25519,
        bip32_derivation_path,
        sizeof(bip32_derivation_path) / sizeof(uint32_t),
        privateKeyData,
        NULL);

    cx_ecfp_init_private_key(
        CX_CURVE_Ed25519,
        privateKeyData,
        32,
        &privateKey);

    os_memset(privateKeyData, 0, sizeof(privateKeyData));

    cx_ecfp_generate_pair(CX_CURVE_Ed25519,
                          &publicKey,
                          &privateKey,
                          1);

    unsigned int info = 0;

    length = cx_eddsa_sign(&privateKey,
                           0,
                           CX_SHA512,
                           message_digest,
                           sizeof(message_digest),
                           NULL,
                           0,
                           signature,
                           sizeof(signature),
                           &info);

    if (info & CX_ECCINFO_PARITY_ODD) {
        signature[0] = 0x01;
    }
    os_memset(&privateKey, 0, sizeof(privateKey));

    return cx_eddsa_verify(&publicKey,
                           0,
                           CX_SHA512,
                           message_digest,
                           sizeof(message_digest),
                           NULL,
                           0,
                           (unsigned char *) signature,
                           length);
}
#endif
