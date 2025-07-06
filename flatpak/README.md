Build
```sh
flatpak-builder --force-clean --user --install-deps-from=flathub --repo=repo --install builddir io.github.hedge_dev.marathonrecomp.json
```

Bundle
```sh
flatpak build-bundle repo io.github.hedge_dev.marathonrecomp.flatpak io.github.hedge_dev.marathonrecomp --runtime-repo=https://flathub.org/repo/flathub.flatpakrepo
```

