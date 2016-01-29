# Introduction #

This guide will help you to build a copy of the latest Vault-Tec Multiplayer Mod source code yourself. Make sure you have all the files from here: http://code.google.com/p/vaultmp/source/browse/trunk

You may want to use SVN software like TortoiseSVN (http://tortoisesvn.net/downloads.html) to receive source updates when available.

# Compiling the source #

To compile vaultmp you will need **MinGW compiler version 4.5** or later. We recommend installing Code::Blocks IDE because there are already project files ready to use.

  * **Preparing Code::Blocks and MinGW**

Code::Blocks can be found here: http://www.codeblocks.org/downloads/26
If you don't have MinGW installed you may want to choose the package which got MinGW bundled.

  * **Upgrading from MinGW 3 to version 4**

In case you followed the above step you still need to upgrade your MinGW to the latest version. To do this, download the package "MinGWUpgrade4.zip" from the downloads section. Read the description.

  * **Compiling vaultmp**

Use the .cbp files to open the projects. Vault-Tec Multiplayer Mod consists of five parts:

  1. _vaultmp.cbp_
  1. _vaultserver/vaultserver.cbp_
  1. _vaultmaster/vaultmaster.cbp_
  1. _vaultmpdll/vaultmpDLL.cbp_
  1. _vaultgui/vaultgui.cbp_

In Code::Blocks, choose "Build" to compile a project.