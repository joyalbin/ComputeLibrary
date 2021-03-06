/*
 * Copyright (c) 2019 Arm Limited.
 *
 * SPDX-License-Identifier: MIT
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#ifdef __aarch64__

#include "../../bfloat.hpp"
#include "../../asmlib.hpp"

namespace arm_gemm {

void a64_interleaved_bf16fp32_mmla_12x8(const bfloat16 *Apanel, const bfloat16 *Bpanel, float *Cpanel, int ablocks, int bblocks, int K) {
    const bfloat16 *a_ptr = Apanel;
    float *c_ptr = Cpanel;

    K /= 4;
    const long loops_count = (K / 2) - 1;
    const long tails_count = K % 2;

    for (int yb=0; yb<ablocks; yb++) {
        const bfloat16 *a_ptr0 = a_ptr;
        const bfloat16 *b_ptr = Bpanel;

        for (int xb=0; xb<bblocks; xb++) {
            a_ptr = a_ptr0;
            long loops = loops_count;
            long tails = tails_count;

            __asm __volatile (
                "movi v8.4s, #0\n"
                "ldr q0, [%[a_ptr]]\n"
                "movi v9.4s, #0\n"
                "ldr q4, [%[b_ptr]]\n"
                "movi v10.4s, #0\n"
                "ldr q1, [%[a_ptr], #0x10]\n"
                "movi v11.4s, #0\n"
                "ldr q5, [%[b_ptr], #0x10]\n"
                "movi v12.4s, #0\n"
                "ldr q2, [%[a_ptr], #0x20]\n"
                "movi v13.4s, #0\n"
                "ldr q6, [%[b_ptr], #0x20]\n"
                "movi v14.4s, #0\n"
                "ldr q3, [%[a_ptr], #0x30]\n"
                "movi v15.4s, #0\n"
                "ldr q7, [%[b_ptr], #0x30]\n"
                "movi v16.4s, #0\n"
                "add %[a_ptr], %[a_ptr], #0x40\n"
                "movi v17.4s, #0\n"
                "add %[b_ptr], %[b_ptr], #0x40\n"
                "movi v18.4s, #0\n"
                "movi v19.4s, #0\n"
                "movi v20.4s, #0\n"
                "movi v21.4s, #0\n"
                "movi v22.4s, #0\n"
                "movi v23.4s, #0\n"
                "movi v24.4s, #0\n"
                "movi v25.4s, #0\n"
                "movi v26.4s, #0\n"
                "movi v27.4s, #0\n"
                "movi v28.4s, #0\n"
                "movi v29.4s, #0\n"
                "movi v30.4s, #0\n"
                "movi v31.4s, #0\n"
                "cbz %[loops], 1f\n"
                "2:\n"
                ".inst 0x6e44ec08 // bfmmla v8.4s, v0.8h, v4.8h\n"
                "subs %[loops], %[loops], #0x1\n"
                ".inst 0x6e44ec2e // bfmmla v14.4s, v1.8h, v4.8h\n"
                ".inst 0x6e44ec54 // bfmmla v20.4s, v2.8h, v4.8h\n"
                ".inst 0x6e44ec7a // bfmmla v26.4s, v3.8h, v4.8h\n"
                "ldr q4, [%[b_ptr]]\n"
                ".inst 0x6e45ec09 // bfmmla v9.4s, v0.8h, v5.8h\n"
                ".inst 0x6e45ec2f // bfmmla v15.4s, v1.8h, v5.8h\n"
                ".inst 0x6e45ec55 // bfmmla v21.4s, v2.8h, v5.8h\n"
                ".inst 0x6e45ec7b // bfmmla v27.4s, v3.8h, v5.8h\n"
                "ldr q5, [%[b_ptr], #0x10]\n"
                ".inst 0x6e46ec0a // bfmmla v10.4s, v0.8h, v6.8h\n"
                ".inst 0x6e46ec30 // bfmmla v16.4s, v1.8h, v6.8h\n"
                ".inst 0x6e46ec56 // bfmmla v22.4s, v2.8h, v6.8h\n"
                ".inst 0x6e46ec7c // bfmmla v28.4s, v3.8h, v6.8h\n"
                "ldr q6, [%[b_ptr], #0x20]\n"
                ".inst 0x6e47ec0b // bfmmla v11.4s, v0.8h, v7.8h\n"
                ".inst 0x6e47ec31 // bfmmla v17.4s, v1.8h, v7.8h\n"
                ".inst 0x6e47ec57 // bfmmla v23.4s, v2.8h, v7.8h\n"
                ".inst 0x6e47ec7d // bfmmla v29.4s, v3.8h, v7.8h\n"
                "ldr q7, [%[b_ptr], #0x30]\n"
                ".inst 0x6e44ec0c // bfmmla v12.4s, v0.8h, v4.8h\n"
                ".inst 0x6e44ec32 // bfmmla v18.4s, v1.8h, v4.8h\n"
                ".inst 0x6e44ec58 // bfmmla v24.4s, v2.8h, v4.8h\n"
                ".inst 0x6e44ec7e // bfmmla v30.4s, v3.8h, v4.8h\n"
                "ldr q4, [%[b_ptr], #0x40]\n"
                ".inst 0x6e45ec0d // bfmmla v13.4s, v0.8h, v5.8h\n"
                "ldr q0, [%[a_ptr]]\n"
                ".inst 0x6e45ec33 // bfmmla v19.4s, v1.8h, v5.8h\n"
                "ldr q1, [%[a_ptr], #0x10]\n"
                ".inst 0x6e45ec59 // bfmmla v25.4s, v2.8h, v5.8h\n"
                "ldr q2, [%[a_ptr], #0x20]\n"
                ".inst 0x6e45ec7f // bfmmla v31.4s, v3.8h, v5.8h\n"
                "ldr q5, [%[b_ptr], #0x50]\n"
                ".inst 0x6e46ec08 // bfmmla v8.4s, v0.8h, v6.8h\n"
                "ldr q3, [%[a_ptr], #0x30]\n"
                ".inst 0x6e46ec2e // bfmmla v14.4s, v1.8h, v6.8h\n"
                "add %[a_ptr], %[a_ptr], #0x80\n"
                ".inst 0x6e46ec54 // bfmmla v20.4s, v2.8h, v6.8h\n"
                "add %[b_ptr], %[b_ptr], #0xc0\n"
                ".inst 0x6e46ec7a // bfmmla v26.4s, v3.8h, v6.8h\n"
                "ldr q6, [%[b_ptr], #-0x60]\n"
                ".inst 0x6e47ec09 // bfmmla v9.4s, v0.8h, v7.8h\n"
                ".inst 0x6e47ec2f // bfmmla v15.4s, v1.8h, v7.8h\n"
                ".inst 0x6e47ec55 // bfmmla v21.4s, v2.8h, v7.8h\n"
                ".inst 0x6e47ec7b // bfmmla v27.4s, v3.8h, v7.8h\n"
                "ldr q7, [%[b_ptr], #-0x50]\n"
                ".inst 0x6e44ec0a // bfmmla v10.4s, v0.8h, v4.8h\n"
                ".inst 0x6e44ec30 // bfmmla v16.4s, v1.8h, v4.8h\n"
                ".inst 0x6e44ec56 // bfmmla v22.4s, v2.8h, v4.8h\n"
                ".inst 0x6e44ec7c // bfmmla v28.4s, v3.8h, v4.8h\n"
                "ldr q4, [%[b_ptr], #-0x40]\n"
                ".inst 0x6e45ec0b // bfmmla v11.4s, v0.8h, v5.8h\n"
                ".inst 0x6e45ec31 // bfmmla v17.4s, v1.8h, v5.8h\n"
                ".inst 0x6e45ec57 // bfmmla v23.4s, v2.8h, v5.8h\n"
                ".inst 0x6e45ec7d // bfmmla v29.4s, v3.8h, v5.8h\n"
                "ldr q5, [%[b_ptr], #-0x30]\n"
                ".inst 0x6e46ec0c // bfmmla v12.4s, v0.8h, v6.8h\n"
                ".inst 0x6e46ec32 // bfmmla v18.4s, v1.8h, v6.8h\n"
                ".inst 0x6e46ec58 // bfmmla v24.4s, v2.8h, v6.8h\n"
                ".inst 0x6e46ec7e // bfmmla v30.4s, v3.8h, v6.8h\n"
                "ldr q6, [%[b_ptr], #-0x20]\n"
                ".inst 0x6e47ec0d // bfmmla v13.4s, v0.8h, v7.8h\n"
                "ldr q0, [%[a_ptr], #-0x40]\n"
                ".inst 0x6e47ec33 // bfmmla v19.4s, v1.8h, v7.8h\n"
                "ldr q1, [%[a_ptr], #-0x30]\n"
                ".inst 0x6e47ec59 // bfmmla v25.4s, v2.8h, v7.8h\n"
                "ldr q2, [%[a_ptr], #-0x20]\n"
                ".inst 0x6e47ec7f // bfmmla v31.4s, v3.8h, v7.8h\n"
                "ldr q7, [%[b_ptr], #-0x10]\n"
                "ldr q3, [%[a_ptr], #-0x10]\n"
                "b.ne 2b\n"
                "1:\n"
                "cbz %[tails], 3f\n"
                ".inst 0x6e44ec08 // bfmmla v8.4s, v0.8h, v4.8h\n"
                ".inst 0x6e44ec2e // bfmmla v14.4s, v1.8h, v4.8h\n"
                ".inst 0x6e44ec54 // bfmmla v20.4s, v2.8h, v4.8h\n"
                ".inst 0x6e44ec7a // bfmmla v26.4s, v3.8h, v4.8h\n"
                "ldr q4, [%[b_ptr]]\n"
                ".inst 0x6e45ec09 // bfmmla v9.4s, v0.8h, v5.8h\n"
                ".inst 0x6e45ec2f // bfmmla v15.4s, v1.8h, v5.8h\n"
                ".inst 0x6e45ec55 // bfmmla v21.4s, v2.8h, v5.8h\n"
                ".inst 0x6e45ec7b // bfmmla v27.4s, v3.8h, v5.8h\n"
                "ldr q5, [%[b_ptr], #0x10]\n"
                ".inst 0x6e46ec0a // bfmmla v10.4s, v0.8h, v6.8h\n"
                ".inst 0x6e46ec30 // bfmmla v16.4s, v1.8h, v6.8h\n"
                ".inst 0x6e46ec56 // bfmmla v22.4s, v2.8h, v6.8h\n"
                ".inst 0x6e46ec7c // bfmmla v28.4s, v3.8h, v6.8h\n"
                "ldr q6, [%[b_ptr], #0x20]\n"
                ".inst 0x6e47ec0b // bfmmla v11.4s, v0.8h, v7.8h\n"
                ".inst 0x6e47ec31 // bfmmla v17.4s, v1.8h, v7.8h\n"
                ".inst 0x6e47ec57 // bfmmla v23.4s, v2.8h, v7.8h\n"
                ".inst 0x6e47ec7d // bfmmla v29.4s, v3.8h, v7.8h\n"
                "ldr q7, [%[b_ptr], #0x30]\n"
                ".inst 0x6e44ec0c // bfmmla v12.4s, v0.8h, v4.8h\n"
                ".inst 0x6e44ec32 // bfmmla v18.4s, v1.8h, v4.8h\n"
                ".inst 0x6e44ec58 // bfmmla v24.4s, v2.8h, v4.8h\n"
                ".inst 0x6e44ec7e // bfmmla v30.4s, v3.8h, v4.8h\n"
                "ldr q4, [%[b_ptr], #0x40]\n"
                ".inst 0x6e45ec0d // bfmmla v13.4s, v0.8h, v5.8h\n"
                "ldr q0, [%[a_ptr]]\n"
                ".inst 0x6e45ec33 // bfmmla v19.4s, v1.8h, v5.8h\n"
                "ldr q1, [%[a_ptr], #0x10]\n"
                ".inst 0x6e45ec59 // bfmmla v25.4s, v2.8h, v5.8h\n"
                "ldr q2, [%[a_ptr], #0x20]\n"
                ".inst 0x6e45ec7f // bfmmla v31.4s, v3.8h, v5.8h\n"
                "ldr q5, [%[b_ptr], #0x50]\n"
                ".inst 0x6e46ec08 // bfmmla v8.4s, v0.8h, v6.8h\n"
                "ldr q3, [%[a_ptr], #0x30]\n"
                ".inst 0x6e46ec2e // bfmmla v14.4s, v1.8h, v6.8h\n"
                "add %[a_ptr], %[a_ptr], #0x80\n"
                ".inst 0x6e46ec54 // bfmmla v20.4s, v2.8h, v6.8h\n"
                "add %[b_ptr], %[b_ptr], #0xe0\n"
                ".inst 0x6e46ec7a // bfmmla v26.4s, v3.8h, v6.8h\n"
                "ldr q6, [%[b_ptr], #-0x80]\n"
                ".inst 0x6e47ec09 // bfmmla v9.4s, v0.8h, v7.8h\n"
                ".inst 0x6e47ec2f // bfmmla v15.4s, v1.8h, v7.8h\n"
                ".inst 0x6e47ec55 // bfmmla v21.4s, v2.8h, v7.8h\n"
                ".inst 0x6e47ec7b // bfmmla v27.4s, v3.8h, v7.8h\n"
                "ldr q7, [%[b_ptr], #-0x70]\n"
                ".inst 0x6e44ec0a // bfmmla v10.4s, v0.8h, v4.8h\n"
                ".inst 0x6e44ec30 // bfmmla v16.4s, v1.8h, v4.8h\n"
                ".inst 0x6e44ec56 // bfmmla v22.4s, v2.8h, v4.8h\n"
                ".inst 0x6e44ec7c // bfmmla v28.4s, v3.8h, v4.8h\n"
                "ldr q4, [%[b_ptr], #-0x60]\n"
                ".inst 0x6e45ec0b // bfmmla v11.4s, v0.8h, v5.8h\n"
                ".inst 0x6e45ec31 // bfmmla v17.4s, v1.8h, v5.8h\n"
                ".inst 0x6e45ec57 // bfmmla v23.4s, v2.8h, v5.8h\n"
                ".inst 0x6e45ec7d // bfmmla v29.4s, v3.8h, v5.8h\n"
                "ldr q5, [%[b_ptr], #-0x50]\n"
                ".inst 0x6e46ec0c // bfmmla v12.4s, v0.8h, v6.8h\n"
                ".inst 0x6e46ec32 // bfmmla v18.4s, v1.8h, v6.8h\n"
                ".inst 0x6e46ec58 // bfmmla v24.4s, v2.8h, v6.8h\n"
                ".inst 0x6e46ec7e // bfmmla v30.4s, v3.8h, v6.8h\n"
                "ldr q6, [%[b_ptr], #-0x40]\n"
                ".inst 0x6e47ec0d // bfmmla v13.4s, v0.8h, v7.8h\n"
                "ldr q0, [%[a_ptr], #-0x40]\n"
                ".inst 0x6e47ec33 // bfmmla v19.4s, v1.8h, v7.8h\n"
                "ldr q1, [%[a_ptr], #-0x30]\n"
                ".inst 0x6e47ec59 // bfmmla v25.4s, v2.8h, v7.8h\n"
                "ldr q2, [%[a_ptr], #-0x20]\n"
                ".inst 0x6e47ec7f // bfmmla v31.4s, v3.8h, v7.8h\n"
                "ldr q7, [%[b_ptr], #-0x30]\n"
                ".inst 0x6e44ec08 // bfmmla v8.4s, v0.8h, v4.8h\n"
                "ldr q3, [%[a_ptr], #-0x10]\n"
                ".inst 0x6e44ec2e // bfmmla v14.4s, v1.8h, v4.8h\n"
                ".inst 0x6e44ec54 // bfmmla v20.4s, v2.8h, v4.8h\n"
                ".inst 0x6e45ec09 // bfmmla v9.4s, v0.8h, v5.8h\n"
                ".inst 0x6e44ec7a // bfmmla v26.4s, v3.8h, v4.8h\n"
                "ldr q4, [%[b_ptr], #-0x20]\n"
                ".inst 0x6e45ec2f // bfmmla v15.4s, v1.8h, v5.8h\n"
                ".inst 0x6e45ec55 // bfmmla v21.4s, v2.8h, v5.8h\n"
                ".inst 0x6e45ec7b // bfmmla v27.4s, v3.8h, v5.8h\n"
                "ldr q5, [%[b_ptr], #-0x10]\n"
                ".inst 0x6e46ec0a // bfmmla v10.4s, v0.8h, v6.8h\n"
                ".inst 0x6e46ec30 // bfmmla v16.4s, v1.8h, v6.8h\n"
                ".inst 0x6e46ec56 // bfmmla v22.4s, v2.8h, v6.8h\n"
                ".inst 0x6e46ec7c // bfmmla v28.4s, v3.8h, v6.8h\n"
                "uzp1 v6.2d, v14.2d, v15.2d\n"
                ".inst 0x6e47ec0b // bfmmla v11.4s, v0.8h, v7.8h\n"
                ".inst 0x6e47ec31 // bfmmla v17.4s, v1.8h, v7.8h\n"
                ".inst 0x6e47ec57 // bfmmla v23.4s, v2.8h, v7.8h\n"
                ".inst 0x6e47ec7d // bfmmla v29.4s, v3.8h, v7.8h\n"
                ".inst 0x6e44ec0c // bfmmla v12.4s, v0.8h, v4.8h\n"
                "uzp1 v7.2d, v16.2d, v17.2d\n"
                ".inst 0x6e44ec32 // bfmmla v18.4s, v1.8h, v4.8h\n"
                ".inst 0x6e44ec58 // bfmmla v24.4s, v2.8h, v4.8h\n"
                ".inst 0x6e44ec7e // bfmmla v30.4s, v3.8h, v4.8h\n"
                "uzp2 v4.2d, v10.2d, v11.2d\n"
                ".inst 0x6e45ec0d // bfmmla v13.4s, v0.8h, v5.8h\n"
                "uzp1 v0.2d, v8.2d, v9.2d\n"
                ".inst 0x6e45ec33 // bfmmla v19.4s, v1.8h, v5.8h\n"
                "uzp1 v1.2d, v10.2d, v11.2d\n"
                ".inst 0x6e45ec59 // bfmmla v25.4s, v2.8h, v5.8h\n"
                "str q0, [%[c_ptr]]\n"
                "uzp1 v2.2d, v12.2d, v13.2d\n"
                "uzp1 v0.2d, v18.2d, v19.2d\n"
                ".inst 0x6e45ec7f // bfmmla v31.4s, v3.8h, v5.8h\n"
                "str q1, [%[c_ptr], #0x10]\n"
                "uzp2 v3.2d, v8.2d, v9.2d\n"
                "uzp2 v5.2d, v12.2d, v13.2d\n"
                "uzp2 v1.2d, v14.2d, v15.2d\n"
                "str q2, [%[c_ptr], #0x20]\n"
                "b 4f\n"
                "3:\n"
                ".inst 0x6e44ec08 // bfmmla v8.4s, v0.8h, v4.8h\n"
                "add %[a_ptr], %[a_ptr], #0x40\n"
                ".inst 0x6e44ec2e // bfmmla v14.4s, v1.8h, v4.8h\n"
                "add %[b_ptr], %[b_ptr], #0x80\n"
                ".inst 0x6e44ec54 // bfmmla v20.4s, v2.8h, v4.8h\n"
                ".inst 0x6e44ec7a // bfmmla v26.4s, v3.8h, v4.8h\n"
                "ldr q4, [%[b_ptr], #-0x80]\n"
                ".inst 0x6e45ec09 // bfmmla v9.4s, v0.8h, v5.8h\n"
                ".inst 0x6e45ec2f // bfmmla v15.4s, v1.8h, v5.8h\n"
                ".inst 0x6e45ec55 // bfmmla v21.4s, v2.8h, v5.8h\n"
                ".inst 0x6e45ec7b // bfmmla v27.4s, v3.8h, v5.8h\n"
                "ldr q5, [%[b_ptr], #-0x70]\n"
                ".inst 0x6e46ec0a // bfmmla v10.4s, v0.8h, v6.8h\n"
                ".inst 0x6e46ec30 // bfmmla v16.4s, v1.8h, v6.8h\n"
                ".inst 0x6e46ec56 // bfmmla v22.4s, v2.8h, v6.8h\n"
                ".inst 0x6e46ec7c // bfmmla v28.4s, v3.8h, v6.8h\n"
                "ldr q6, [%[b_ptr], #-0x60]\n"
                ".inst 0x6e47ec0b // bfmmla v11.4s, v0.8h, v7.8h\n"
                ".inst 0x6e47ec31 // bfmmla v17.4s, v1.8h, v7.8h\n"
                ".inst 0x6e47ec57 // bfmmla v23.4s, v2.8h, v7.8h\n"
                ".inst 0x6e47ec7d // bfmmla v29.4s, v3.8h, v7.8h\n"
                "ldr q7, [%[b_ptr], #-0x50]\n"
                ".inst 0x6e44ec0c // bfmmla v12.4s, v0.8h, v4.8h\n"
                ".inst 0x6e44ec32 // bfmmla v18.4s, v1.8h, v4.8h\n"
                ".inst 0x6e44ec58 // bfmmla v24.4s, v2.8h, v4.8h\n"
                ".inst 0x6e44ec7e // bfmmla v30.4s, v3.8h, v4.8h\n"
                "ldr q4, [%[b_ptr], #-0x40]\n"
                ".inst 0x6e45ec0d // bfmmla v13.4s, v0.8h, v5.8h\n"
                "ldr q0, [%[a_ptr], #-0x40]\n"
                ".inst 0x6e45ec33 // bfmmla v19.4s, v1.8h, v5.8h\n"
                "ldr q1, [%[a_ptr], #-0x30]\n"
                ".inst 0x6e45ec59 // bfmmla v25.4s, v2.8h, v5.8h\n"
                "ldr q2, [%[a_ptr], #-0x20]\n"
                ".inst 0x6e45ec7f // bfmmla v31.4s, v3.8h, v5.8h\n"
                "ldr q5, [%[b_ptr], #-0x30]\n"
                ".inst 0x6e46ec08 // bfmmla v8.4s, v0.8h, v6.8h\n"
                "ldr q3, [%[a_ptr], #-0x10]\n"
                ".inst 0x6e46ec2e // bfmmla v14.4s, v1.8h, v6.8h\n"
                ".inst 0x6e46ec54 // bfmmla v20.4s, v2.8h, v6.8h\n"
                ".inst 0x6e47ec09 // bfmmla v9.4s, v0.8h, v7.8h\n"
                ".inst 0x6e46ec7a // bfmmla v26.4s, v3.8h, v6.8h\n"
                "ldr q6, [%[b_ptr], #-0x20]\n"
                ".inst 0x6e47ec2f // bfmmla v15.4s, v1.8h, v7.8h\n"
                ".inst 0x6e47ec55 // bfmmla v21.4s, v2.8h, v7.8h\n"
                ".inst 0x6e47ec7b // bfmmla v27.4s, v3.8h, v7.8h\n"
                "ldr q7, [%[b_ptr], #-0x10]\n"
                ".inst 0x6e44ec0a // bfmmla v10.4s, v0.8h, v4.8h\n"
                ".inst 0x6e44ec30 // bfmmla v16.4s, v1.8h, v4.8h\n"
                ".inst 0x6e44ec56 // bfmmla v22.4s, v2.8h, v4.8h\n"
                ".inst 0x6e44ec7c // bfmmla v28.4s, v3.8h, v4.8h\n"
                ".inst 0x6e45ec0b // bfmmla v11.4s, v0.8h, v5.8h\n"
                ".inst 0x6e45ec31 // bfmmla v17.4s, v1.8h, v5.8h\n"
                ".inst 0x6e45ec57 // bfmmla v23.4s, v2.8h, v5.8h\n"
                ".inst 0x6e45ec7d // bfmmla v29.4s, v3.8h, v5.8h\n"
                "uzp2 v4.2d, v10.2d, v11.2d\n"
                ".inst 0x6e46ec0c // bfmmla v12.4s, v0.8h, v6.8h\n"
                ".inst 0x6e46ec32 // bfmmla v18.4s, v1.8h, v6.8h\n"
                ".inst 0x6e46ec58 // bfmmla v24.4s, v2.8h, v6.8h\n"
                ".inst 0x6e46ec7e // bfmmla v30.4s, v3.8h, v6.8h\n"
                "uzp1 v6.2d, v14.2d, v15.2d\n"
                ".inst 0x6e47ec0d // bfmmla v13.4s, v0.8h, v7.8h\n"
                "uzp1 v0.2d, v8.2d, v9.2d\n"
                ".inst 0x6e47ec33 // bfmmla v19.4s, v1.8h, v7.8h\n"
                "uzp1 v1.2d, v10.2d, v11.2d\n"
                "uzp2 v5.2d, v12.2d, v13.2d\n"
                "str q0, [%[c_ptr]]\n"
                ".inst 0x6e47ec59 // bfmmla v25.4s, v2.8h, v7.8h\n"
                "uzp1 v2.2d, v12.2d, v13.2d\n"
                "uzp1 v0.2d, v18.2d, v19.2d\n"
                "str q1, [%[c_ptr], #0x10]\n"
                "uzp2 v1.2d, v14.2d, v15.2d\n"
                ".inst 0x6e47ec7f // bfmmla v31.4s, v3.8h, v7.8h\n"
                "uzp2 v3.2d, v8.2d, v9.2d\n"
                "str q2, [%[c_ptr], #0x20]\n"
                "uzp1 v7.2d, v16.2d, v17.2d\n"
                "4:\n"
                "uzp2 v2.2d, v16.2d, v17.2d\n"
                "str q3, [%[c_ptr], #0x30]\n"
                "uzp2 v3.2d, v18.2d, v19.2d\n"
                "str q4, [%[c_ptr], #0x40]\n"
                "uzp1 v4.2d, v20.2d, v21.2d\n"
                "str q5, [%[c_ptr], #0x50]\n"
                "uzp1 v5.2d, v22.2d, v23.2d\n"
                "str q6, [%[c_ptr], #0x60]\n"
                "uzp1 v6.2d, v24.2d, v25.2d\n"
                "str q7, [%[c_ptr], #0x70]\n"
                "uzp2 v7.2d, v20.2d, v21.2d\n"
                "str q0, [%[c_ptr], #0x80]\n"
                "uzp2 v0.2d, v22.2d, v23.2d\n"
                "str q1, [%[c_ptr], #0x90]\n"
                "uzp2 v1.2d, v24.2d, v25.2d\n"
                "str q2, [%[c_ptr], #0xa0]\n"
                "uzp1 v2.2d, v26.2d, v27.2d\n"
                "str q3, [%[c_ptr], #0xb0]\n"
                "uzp1 v3.2d, v28.2d, v29.2d\n"
                "str q4, [%[c_ptr], #0xc0]\n"
                "uzp1 v4.2d, v30.2d, v31.2d\n"
                "str q5, [%[c_ptr], #0xd0]\n"
                "uzp2 v5.2d, v26.2d, v27.2d\n"
                "str q6, [%[c_ptr], #0xe0]\n"
                "uzp2 v6.2d, v28.2d, v29.2d\n"
                "str q7, [%[c_ptr], #0xf0]\n"
                "uzp2 v7.2d, v30.2d, v31.2d\n"
                "str q0, [%[c_ptr], #0x100]\n"
                "str q1, [%[c_ptr], #0x110]\n"
                "str q2, [%[c_ptr], #0x120]\n"
                "str q3, [%[c_ptr], #0x130]\n"
                "str q4, [%[c_ptr], #0x140]\n"
                "str q5, [%[c_ptr], #0x150]\n"
                "str q6, [%[c_ptr], #0x160]\n"
                "str q7, [%[c_ptr], #0x170]\n"
                "add %[c_ptr], %[c_ptr], #0x180\n"
            : [a_ptr] "+r" (a_ptr), [b_ptr] "+r" (b_ptr), [c_ptr] "+r" (c_ptr),
              [loops] "+r" (loops), [tails] "+r" (tails)
            :
            : "v0", "v1", "v2", "v3", "v4", "v5", "v6", "v7", "v8", "v9", "v10", "v11", "v12", "v13", "v14", "v15", "v16", "v17", "v18", "v19", "v20", "v21", "v22", "v23", "v24", "v25", "v26", "v27", "v28", "v29", "v30", "v31", "cc", "memory"
            );
        }
    }
}

} // namespace arm_gemm

#endif // __aarch64__
