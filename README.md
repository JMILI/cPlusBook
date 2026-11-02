# cPlusBook · C++ 知识点简要总结

用 [mdBook](https://rust-lang.github.io/mdBook/) 构建的 C++ 笔记站，线上通过 Vercel 发布。

- 内容目录：`src/SUMMARY.md`（书籍首页是 `src/README.md`）
- 样式：`css/custom.css`（`book.toml` 里以 `additional-css` 引入）
- 静态产物：`book/`（**不入库**，`.gitignore` 已忽略；本地与线上都由 mdbook 现构建）

## 本地构建

```bash
cd deployProject/cPlusBook
./build.sh          # 首次会下载 mdbook v0.4.36 到当前目录，然后执行 ./mdbook build
```

构建完成后用任意静态服务器打开 `book/index.html`，或在 ProjectAccess 里：

```bash
bash pa.sh up cplusbook      # 本地 http://127.0.0.1:4001/
```

## 编辑内容

1. 改 `src/` 下的 Markdown（新增页面记得登记到 `src/SUMMARY.md`）
2. 重跑 `./build.sh` 重新生成 `book/`
3. 提交 `src/` 与 `book/` 的改动

> `book/` 是构建产物且被 `.gitignore` 忽略：Vercel 用 `vercel.json` 里的 `buildCommand`（`./build.sh`）现场生成，`ProjectAccess` 的 4001 端口则托管本地已有的 `book/`。**只改 `src/` 不重建**，本地 4001 的页面不会更新。

## 部署

- `vercel.json` 已配置，推送仓库后由 Vercel 直接发布 `book/`。
- 本地预览、隧道（`sealtun cplusbook-ui`）都由 `ProjectAccess` 的 `start_all.sh` / `pa.sh` 统一管理。
