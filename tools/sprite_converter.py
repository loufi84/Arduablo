#!/usr/bin/env python3

from pathlib import Path
import argparse
import re

from PIL import Image


def cpp_name(filename: str) -> str:
    name = Path(filename).stem.upper()
    name = re.sub(r"[^A-Z0-9_]", "_", name)

    if name and name[0].isdigit():
        name = "_" + name

    return name


def normalize_sprite(
    image: Image.Image,
    width: int,
    height: int
) -> Image.Image:

    image = image.convert("RGBA")

    alpha = image.getchannel("A")
    bbox = alpha.getbbox()

    if bbox is None:
        raise ValueError(
            "Image complètement transparente."
        )

    image = image.crop(bbox)

    image.thumbnail(
        (width, height),
        Image.Resampling.NEAREST
    )

    result = Image.new(
        "RGBA",
        (width, height),
        (0, 0, 0, 0)
    )

    x = (width - image.width) // 2
    y = (height - image.height) // 2

    result.alpha_composite(
        image,
        (x, y)
    )


    # Conversion en vrai monochrome
    pixels = result.load()

    for py in range(height):
        for px in range(width):

            r, g, b, a = pixels[px, py]

            if a >= 96:
                pixels[px, py] = (
                    255,
                    255,
                    255,
                    255
                )
            else:
                pixels[px, py] = (
                    0,
                    0,
                    0,
                    0
                )

    return result


def convert_image(
    image: Image.Image
) -> list[int]:

    width, height = image.size

    pixels = image.load()

    data = []

    pages = (height + 7) // 8


    for page in range(pages):

        for x in range(width):

            value = 0

            for bit in range(8):

                y = page * 8 + bit

                if y >= height:
                    continue

                _, _, _, alpha = pixels[x, y]

                if alpha >= 128:
                    value |= 1 << bit

            data.append(value)

    return data


def sprite_cpp(
    name: str,
    width: int,
    height: int,
    data: list[int]
) -> str:

    lines = []

    lines.append(
        f"const uint8_t {name}[] PROGMEM = {{"
    )

    lines.append(
        f"    {width}, {height},"
    )


    for i in range(
        0,
        len(data),
        8
    ):

        chunk = data[i:i + 8]

        values = ", ".join(
            f"0x{value:02X}"
            for value in chunk
        )

        suffix = (
            ","
            if i + 8 < len(data)
            else ""
        )

        lines.append(
            f"    {values}{suffix}"
        )


    lines.append("};")

    return "\n".join(lines)


def convert_file(
    path: Path,
    width: int,
    height: int,
    name: str | None = None,
    png_out: Path | None = None
):

    image = Image.open(path)

    sprite = normalize_sprite(
        image,
        width,
        height
    )


    if png_out is not None:

        png_out.parent.mkdir(
            parents=True,
            exist_ok=True
        )

        sprite.save(
            png_out
        )


    data = convert_image(
        sprite
    )


    sprite_name = (
        name.upper()
        if name
        else cpp_name(path.name)
    )


    return (
        sprite_name,
        sprite,
        data
    )


def generate_header(
    directory: Path,
    output: Path,
    width: int,
    height: int
):

    png_files = sorted(
        directory.glob("*.png")
    )


    if not png_files:

        raise SystemExit(
            f"Aucun PNG trouvé dans {directory}"
        )


    parts = []

    parts.append("#pragma once")
    parts.append("")
    parts.append("#include <Arduino.h>")
    parts.append("")
    parts.append("namespace GameSprites {")
    parts.append("")


    for path in png_files:

        name, _, data = convert_file(
            path,
            width,
            height
        )


        parts.append(
            sprite_cpp(
                name,
                width,
                height,
                data
            )
        )

        parts.append("")


    parts.append(
        "} // namespace GameSprites"
    )

    parts.append("")


    output.parent.mkdir(
        parents=True,
        exist_ok=True
    )


    output.write_text(
        "\n".join(parts),
        encoding="utf-8"
    )


    print(
        f"{len(png_files)} sprites générés -> {output}"
    )


def main():

    parser = argparse.ArgumentParser(
        description=(
            "Convertisseur PNG -> sprites Arduboy"
        )
    )


    parser.add_argument(
        "input",
        type=Path,
        help="PNG ou dossier contenant des PNG"
    )


    parser.add_argument(
        "--name",
        help="Nom du sprite C++"
    )


    parser.add_argument(
        "--width",
        type=int,
        default=8
    )


    parser.add_argument(
        "--height",
        type=int,
        default=8
    )


    parser.add_argument(
        "--png-out",
        type=Path,
        help="Sauvegarde aussi le PNG normalisé"
    )


    parser.add_argument(
        "--header",
        type=Path,
        help="Génère directement un header C++"
    )


    args = parser.parse_args()


    # ========================================================
    # MODE DOSSIER
    # ========================================================

    if args.input.is_dir():

        if args.header is None:
            raise SystemExit(
                "En mode dossier, utilise --header."
            )


        generate_header(
            args.input,
            args.header,
            args.width,
            args.height
        )

        return


    # ========================================================
    # MODE FICHIER
    # ========================================================

    if not args.input.exists():

        raise SystemExit(
            f"Fichier introuvable : {args.input}"
        )


    name, _, data = convert_file(
        args.input,
        args.width,
        args.height,
        args.name,
        args.png_out
    )


    print(
        sprite_cpp(
            name,
            args.width,
            args.height,
            data
        )
    )


if __name__ == "__main__":
    main()
