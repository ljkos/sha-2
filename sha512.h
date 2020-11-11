
#ifndef SHA512_H
#define SHA512_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>



int sha512(uint64_t hash[8], const void *data, size_t bytes) {
    size_t bits = bytes * 8;
    size_t zeros = (bits % 1024 > 895 ? 1920 : 896) - (bits % 1024 + 1);
    size_t total_bits = bits + 1 + zeros + 128;
    size_t total_bytes = total_bits / 8;
    
    size_t block_count = total_bits / 1024;
    
    uint64_t *blocks = malloc(total_bytes);
    if (!blocks)
        return -1;
    
    memset(blocks, 0, total_bytes);
    
    uint8_t *byte = (uint8_t *) data;
    
    for (size_t i = 0; i < bytes; i++) {
        blocks[i / 8] |= (uint64_t) byte[i] << ((7 - (i % 8)) * 8);
    }
    
    blocks[bytes / 8] |= 0x8000000000000000 >> (bits % 64);
    blocks[block_count * 16 - 1] = (uint64_t) bits;
    
    uint64_t ha[8] = {
        0x6a09e667f3bcc908,
        0xbb67ae8584caa73b,
        0x3c6ef372fe94f82b,
        0xa54ff53a5f1d36f1,
        0x510e527fade682d1,
        0x9b05688c2b3e6c1f,
        0x1f83d9abfb41bd6b,
        0x5be0cd19137e2179
    };
    
    uint64_t k[80] = {
        0x428a2f98d728ae22, 0x7137449123ef65cd, 0xb5c0fbcfec4d3b2f, 0xe9b5dba58189dbbc,
        0x3956c25bf348b538, 0x59f111f1b605d019, 0x923f82a4af194f9b, 0xab1c5ed5da6d8118,
        0xd807aa98a3030242, 0x12835b0145706fbe, 0x243185be4ee4b28c, 0x550c7dc3d5ffb4e2,
        0x72be5d74f27b896f, 0x80deb1fe3b1696b1, 0x9bdc06a725c71235, 0xc19bf174cf692694,
        0xe49b69c19ef14ad2, 0xefbe4786384f25e3, 0x0fc19dc68b8cd5b5, 0x240ca1cc77ac9c65,
        0x2de92c6f592b0275, 0x4a7484aa6ea6e483, 0x5cb0a9dcbd41fbd4, 0x76f988da831153b5,
        0x983e5152ee66dfab, 0xa831c66d2db43210, 0xb00327c898fb213f, 0xbf597fc7beef0ee4,
        0xc6e00bf33da88fc2, 0xd5a79147930aa725, 0x06ca6351e003826f, 0x142929670a0e6e70,
        0x27b70a8546d22ffc, 0x2e1b21385c26c926, 0x4d2c6dfc5ac42aed, 0x53380d139d95b3df,
        0x650a73548baf63de, 0x766a0abb3c77b2a8, 0x81c2c92e47edaee6, 0x92722c851482353b,
        0xa2bfe8a14cf10364, 0xa81a664bbc423001, 0xc24b8b70d0f89791, 0xc76c51a30654be30,
        0xd192e819d6ef5218, 0xd69906245565a910, 0xf40e35855771202a, 0x106aa07032bbd1b8,
        0x19a4c116b8d2d0c8, 0x1e376c085141ab53, 0x2748774cdf8eeb99, 0x34b0bcb5e19b48a8,
        0x391c0cb3c5c95a63, 0x4ed8aa4ae3418acb, 0x5b9cca4f7763e373, 0x682e6ff3d6b2b8a3,
        0x748f82ee5defb2fc, 0x78a5636f43172f60, 0x84c87814a1f0ab72, 0x8cc702081a6439ec,
        0x90befffa23631e28, 0xa4506cebde82bde9, 0xbef9a3f7b2c67915, 0xc67178f2e372532b,
        0xca273eceea26619c, 0xd186b8c721c0c207, 0xeada7dd6cde0eb1e, 0xf57d4f7fee6ed178,
        0x06f067aa72176fba, 0x0a637dc5a2c898a6, 0x113f9804bef90dae, 0x1b710b35131c471b,
        0x28db77f523047d84, 0x32caab7b40c72493, 0x3c9ebe0a15c9bebc, 0x431d67c49c100d4c,
        0x4cc5d4becb3e42b6, 0x597f299cfc657e2a, 0x5fcb6fab3ad6faec, 0x6c44198c4a475817
    };
    
    uint64_t w[80];
    uint64_t a, b, c, d, e, f, g, h, t1, t2;
    
    for (size_t i = 0; i < block_count; i++) {
        a = ha[0];
        b = ha[1];
        c = ha[2];
        d = ha[3];
        e = ha[4];
        f = ha[5];
        g = ha[6];
        h = ha[7];
        
        for (size_t w_j = 0; w_j < 16; w_j++) {
            w[w_j] = blocks[16 * i + w_j];
        }
        
        for (size_t w_j = 16; w_j < 80; w_j++) {
            w[w_j] = (((w[w_j - 2] >> 19) | (w[w_j - 2] << 45)) ^ ((w[w_j - 2] >> 61) | (w[w_j - 2] << 3)) ^ (w[w_j - 2] >> 6));
            w[w_j] += w[w_j - 7];
            w[w_j] += (((w[w_j - 15] >> 1) | (w[w_j - 15] << 63)) ^ ((w[w_j - 15] >> 8) | (w[w_j - 15] << 56)) ^ (w[w_j - 15] >> 7));
            w[w_j] += w[w_j - 16];
        }
        
        for (size_t j = 0; j < 80; j++) {
            t1 = h + (((e >> 14) | (e << 50)) ^ ((e >> 18) | (e << 46)) ^ ((e >> 41) | (e << 23)));
            t1 += ((e & f) ^ (~e & g)) + k[j] + w[j];
            
            t2 = (((a >> 28) | (a << 36)) ^ ((a >> 34) | (a << 30)) ^ ((a >> 39) | (a << 25)));
            t2 += ((a & b) ^ (a & c) ^ (b & c));
            
            h = g;
            g = f;
            f = e;
            e = d + t1;
            d = c;
            c = b;
            b = a;
            a = t1 + t2;
        }
        
        ha[0] += a;
        ha[1] += b;
        ha[2] += c;
        ha[3] += d;
        ha[4] += e;
        ha[5] += f;
        ha[6] += g;
        ha[7] += h;
    }
    
    memcpy(hash, ha, 64);
    
    free(blocks);
    
    return 0;
}

#endif