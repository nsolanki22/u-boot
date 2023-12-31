// SPDX-License-Identifier: GPL-2.0+
/*
 *
 * (C) Copyright 2000-2003
 * Wolfgang Denk, DENX Software Engineering, wd@denx.de.
 *
 * Copyright (C) 2004-2007, 2012 Freescale Semiconductor, Inc.
 * TsiChung Liew (Tsi-Chung.Liew@freescale.com)
 */

#include <init.h>
#include <net.h>
#include <vsprintf.h>
#include <command.h>
#include <netdev.h>
#include <asm/global_data.h>

#include <asm/immap.h>
#include <asm/io.h>

DECLARE_GLOBAL_DATA_PTR;

int do_reset(struct cmd_tbl *cmdtp, int flag, int argc, char *const argv[])
{
	ccm_t *ccm = (ccm_t *) MMAP_CCM;

	out_8(&ccm->rcr, CCM_RCR_SOFTRST);
	/* we don't return! */
	return 0;
}

#if defined(CONFIG_DISPLAY_CPUINFO)
int print_cpuinfo(void)
{
	ccm_t *ccm = (ccm_t *) MMAP_CCM;
	u16 msk;
	u16 id = 0;
	u8 ver;

	puts("CPU:   ");
	msk = (in_be16(&ccm->cir) >> 6);
	ver = (in_be16(&ccm->cir) & 0x003f);
	switch (msk) {
	case 0x31:
		id = 5235;
		break;
	}

	if (id) {
		char buf1[32], buf2[32];

		printf("Freescale MCF%d (Mask:%01x Version:%x)\n", id, msk,
		       ver);
		printf("       CPU CLK %s MHz BUS CLK %s MHz\n",
		       strmhz(buf1, gd->cpu_clk),
		       strmhz(buf2, gd->bus_clk));
	}

	return 0;
};
#endif /* CONFIG_DISPLAY_CPUINFO */

#if defined(CONFIG_MCFFEC)
/* Default initializations for MCFFEC controllers.  To override,
 * create a board-specific function called:
 *	int board_eth_init(struct bd_info *bis)
 */

int cpu_eth_init(struct bd_info *bis)
{
	return mcffec_initialize(bis);
}
#endif
