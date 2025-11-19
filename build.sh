#!/bin/bash
set -e  # 遇到错误立即退出，方便排查

MDBOOK_VERSION="0.4.36"
MDBOOK_URL="https://github.com/rust-lang/mdBook/releases/download/v${MDBOOK_VERSION}/mdbook-v${MDBOOK_VERSION}-x86_64-unknown-linux-gnu.tar.gz"

# 下载并解压mdBook到当前目录
curl -L $MDBOOK_URL -o mdbook.tar.gz
tar -xzf mdbook.tar.gz
# 现在直接使用 ./mdbook，无需sudo和移动文件
chmod +x ./mdbook

# 验证安装
./mdbook --version

# 构建书籍
./mdbook build

