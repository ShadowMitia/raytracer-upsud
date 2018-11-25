#!/bin/bash

cp ./*.yaml Debug
mkdir assignement_Touchais_Belopopsky
pushd assignement_Touchais_Belopopsky || return
git clone https://github.com/ShadowMitia/raytracer.git raytracer
cp -r raytracer/images .
cp -r raytracer/error-images .
mkdir raytracer-binary
mkdir results
if [ ! -f README.md ]; then
    echo "# Assignement #?\n\nDimitri Belopopsky\\nVirginie Touchais\n\n# Lab session " > README.md
fi

rm RUNME.bat
for i in raytracer-binary/*.yaml; do
    printf "raytracer.exe raytracer-binary\\%s ..\\results\\%s.png\\n" "$(basename $i)" "$(basename $i .yaml)" >> RUNME.bat;
    printf "mspaint results\\%s.png\\n" "$(basename $i .yaml)" >> RUNME.bat;
done
