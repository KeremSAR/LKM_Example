#include <linux/module.h>
#define INCLUDE_VERMAGIC
#include <linux/build-salt.h>
#include <linux/elfnote-lto.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

BUILD_SALT;
BUILD_LTO_INFO;

MODULE_INFO(vermagic, VERMAGIC_STRING);
MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__section(".gnu.linkonce.this_module") = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

#ifdef CONFIG_RETPOLINE
MODULE_INFO(retpoline, "Y");
#endif

static const struct modversion_info ____versions[]
__used __section("__versions") = {
	{ 0xd9726f80, "module_layout" },
	{ 0xd7ed5de7, "single_release" },
	{ 0x91c1631c, "seq_lseek" },
	{ 0xb75bc01, "remove_proc_entry" },
	{ 0xbe8b05f8, "proc_create" },
	{ 0x6b1c962e, "proc_mkdir" },
	{ 0xc5850110, "printk" },
	{ 0xcbd4898c, "fortify_panic" },
	{ 0x37a0cba, "kfree" },
	{ 0x13c49cc2, "_copy_from_user" },
	{ 0xeb233a45, "__kmalloc" },
	{ 0x56470118, "__warn_printk" },
	{ 0x6b10bee1, "_copy_to_user" },
	{ 0x88db9f48, "__check_object_size" },
	{ 0xad280409, "seq_printf" },
	{ 0xe53e512c, "single_open" },
	{ 0xbdfb6dbb, "__fentry__" },
};

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "499F6571CAACB20FB0CB845");
