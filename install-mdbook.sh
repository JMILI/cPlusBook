#!/bin/bash
# 安装mdBook

MDBOOK_VERSION="0.4.36"
MDBOOK_URL="https://github.com/rust-lang/mdBook/releases/download/v${MDBOOK_VERSION}/mdbook-v${MDBOOK_VERSION}-x86_64-unknown-linux-gnu.tar.gz"

# 下载并解压mdBook
wget -O mdbook.tar.gz $MDBOOK_URL
tar -xzf mdbook.tar.gz
sudo mv mdbook /usr/local/bin/

# 验证安装
mdbook --version