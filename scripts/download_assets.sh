#!/bin/bash
mkdir -p assets
cd assets

# sponza
wget https://casual-effects.com/g3d/data10/common/model/crytek_sponza/sponza.zip
unzip -d sponza sponza.zip
rm sponza.zip

# normalmap_test
wget https://storage.googleapis.com/computer-graphics-assets/normalmap_test.zip
unzip normalmap_test.zip
rm normalmap_test.zip

cd ../
