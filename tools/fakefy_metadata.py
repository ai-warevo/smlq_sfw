# =====================================================================
# Batch Process Image & Video Metadata with Authentic Apple Naming
# Converts images to JPEGs with true Windows EXIF tags.
# Injects Apple QuickTime metadata safely into video containers.
# Outputs new copies using Apple's 'IMG_XXXX' layout (Originals Kept).
# =====================================================================
import argparse
import shutil
import sys
from datetime import datetime
from pathlib import Path
from typing import List
import piexif
from PIL import Image, PngImagePlugin
from mutagen.mp4 import MP4


class UniversalMetadataSpoofer:
    """Processes folders by standardizing and creating clean iPhone-structured

    copies of both image and video assets without deleting the source files.
    """

    def __init__(self) -> None:
        self.now = datetime.now()
        self.current_time_exif: str = self.now.strftime("%Y:%m:%d %H:%M:%S")
        self.current_time_video: str = self.now.strftime("%Y-%m-%dT%H:%M:%SZ")

        self.make: str = "Apple"
        self.model: str = "iPhone 16 Pro Max"
        self.software: str = "iOS 18.1"

        self.img_filetypes: List[str] = [".jpg", ".jpeg", ".png", ".tif", ".tiff", ".bmp", ".heic"]
        self.video_filetypes: List[str] = [".mp4", ".mov"]

    def generate_iphone_exif(self) -> bytes:
        """Compiles a standard binary EXIF payload tailored to iPhone 16 Pro Max specs."""
        zeroth_ifd = {
            piexif.ImageIFD.Make: self.make.encode("utf-8"),
            piexif.ImageIFD.Model: self.model.encode("utf-8"),
            piexif.ImageIFD.Software: self.software.encode("utf-8"),
            piexif.ImageIFD.DateTime: self.current_time_exif.encode("utf-8"),
        }

        exif_ifd = {
            piexif.ExifIFD.ExifVersion: b"0232",
            piexif.ExifIFD.ComponentsConfiguration: b"\x01\x02\x03\x00",
            piexif.ExifIFD.DateTimeOriginal: self.current_time_exif.encode("utf-8"),
            piexif.ExifIFD.DateTimeDigitized: self.current_time_exif.encode("utf-8"),
            piexif.ExifIFD.FNumber: (178, 100),
            piexif.ExifIFD.ISOSpeedRatings: 50,
            piexif.ExifIFD.FocalLength: (677, 100),
            piexif.ExifIFD.FocalLengthIn35mmFilm: 24,
            piexif.ExifIFD.LensMake: self.make.encode("utf-8"),
            piexif.ExifIFD.LensModel: "iPhone 16 Pro Max back triple camera 6.77mm f/1.78".encode(
                "utf-8"
            ),
        }

        gps_ifd = {
            piexif.GPSIFD.GPSLatitudeRef: b"N",
            piexif.GPSIFD.GPSLatitude: ((37, 1), (20, 1), (5, 1)),
            piexif.GPSIFD.GPSLongitudeRef: b"W",
            piexif.GPSIFD.GPSLongitude: ((122, 1), (0, 1), (32, 1)),
        }

        exif_dict = {"0th": zeroth_ifd, "Exif": exif_ifd, "GPS": gps_ifd}
        return piexif.dump(exif_dict)

    def process_image(self, src_path: Path, dest_path: Path, exif_bytes: bytes) -> None:
        """Converts input image to true JPEG format at destination path with clean EXIF."""
        with Image.open(src_path) as img:
            if img.mode in ("RGBA", "P"):
                img = img.convert("RGB")
            img.save(dest_path, "JPEG", exif=exif_bytes)

    def process_video(self, src_path: Path, dest_path: Path) -> None:
        """Copies video to target layout path and purges metadata cleanly."""
        shutil.copy2(src_path, dest_path)

        with open(dest_path, "r+b") as f:
            video = MP4(f)
            video.delete(f)
            video["\xa9nam"] = "Core Video"
            video["\xa9day"] = self.current_time_video
            video["\xa9too"] = self.software
            video["\xa9xyz"] = "+37.3349-122.0090/"
            video.save(f)

    def process_folder(self, folder_path: Path) -> None:
        """Iterates through a directory to generate formatted copies matching an Apple device."""
        if not folder_path.exists() or not folder_path.is_dir():
            raise FileNotFoundError(f"Folder directory not found: {folder_path}")

        print(f"[+] Starting batch processing in directory: {folder_path.resolve()}")
        exif_bytes = self.generate_iphone_exif()

        img_count = 0
        video_count = 0
        asset_index = 1

        files_to_process = sorted(list(folder_path.iterdir()), key=lambda x: x.name)

        for path in files_to_process:
            if path.name.startswith("IMG_") and path.suffix.lower() in [".jpg", ".mp4"]:
                continue

            suffix = path.suffix.lower()

            if suffix in self.img_filetypes:
                dest_path = folder_path / f"IMG_{asset_index:04d}.jpg"
                print(f"[+] Copying & Formatting Image: {path.name} -> {dest_path.name}")
                try:
                    self.process_image(path, dest_path, exif_bytes)
                    img_count += 1
                    asset_index += 1
                except Exception as e:
                    print(f"❌ Failed processing image {path.name}: {e}", file=sys.stderr)

            elif suffix in self.video_filetypes:
                dest_path = folder_path / f"IMG_{asset_index:04d}.mp4"
                print(f"[+] Copying & Formatting Video: {path.name} -> {dest_path.name}")
                try:
                    self.process_video(path, dest_path)
                    video_count += 1
                    asset_index += 1
                except Exception as e:
                    print(f"❌ Failed processing video {path.name}: {e}", file=sys.stderr)

        print(f"\n🎉 Successfully finalized layout: {img_count} images and {video_count} videos generated.")


def main() -> None:
    parser = argparse.ArgumentParser(
        description="Batch Convert Images to JPEG and Spoof Image/Video Metadata"
    )
    parser.add_argument(
        "--folder",
        "--f",
        metavar="",
        default="sample_images",
        help="Name or path of folder where images/videos are stored",
        type=str,
    )

    args = parser.parse_args()
    spoofer = UniversalMetadataSpoofer()

    try:
        spoofer.process_folder(Path(args.folder))
    except Exception as e:
        print(f"❌ Execution error: {e}", file=sys.stderr)
        sys.exit(1)


if __name__ == "__main__":
    main()
