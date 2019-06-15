/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Copyright (C) 2018 Google, Inc.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#ifndef GOLDFISH_PIPE_V2_H
#define GOLDFISH_PIPE_V2_H

/* The entry point to the pipe v2 driver */
int goldfish_pipe_device_v2_init(struct platform_device *pdev,
				 char __iomem *base,
				 int irq);

#endif /* #define GOLDFISH_PIPE_V2_H */
