import glob
import os
from PIL import Image

ana_folder  = "./Ch_11221_Selection_False_hST_zoom/"
match_string = "LED_1400"
output_gif = "led_1400_animation.gif"
frame_durations = 500  # or [500, 700, 300, ...] for custom durations per frame




if __name__ == "__main__":
    pattern = os.path.join(ana_folder, f"*{match_string}*")
    image_files = sorted(glob.glob(pattern))

    # Open images
    frames = [Image.open(img) for img in image_files]

    # Save as GIF
    if frames:
        frames[0].save(
            output_gif,
            format="GIF",
            append_images=frames[1:],
            save_all=True,
            duration=frame_durations,
            loop=0
        )
        print(f"GIF created from {len(frames)} images: {output_gif}")
    else:
        print("No matching images found.")
