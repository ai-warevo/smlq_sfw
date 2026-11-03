# H3 Long Shot - Stable Yogi

One free ComfyUI workflow. One start picture and one prompt make one unbroken MiniMax H3 take with voice and sound, up to 40 seconds. A second group brings the take up to 1080p with the same face.

## Files
- `StableYogi-H3-Long-Shot.json` - drag it into ComfyUI.
- `sy-pool-start.png` - the start picture from the video. Put it in ComfyUI's `input` folder.
- `prompt.txt` - the prompt from the video. It is already in the workflow.

## Model files
| Loader | File | Folder |
|---|---|---|
| H3 model | minimax_h3_fl2va_pruned_int8_convrot.safetensors | diffusion_models |
| H3 text encoder, type minimax | qwen3vl_32b_minimax_h3_nvfp4_awq.safetensors | text_encoders |
| H3 video VAE | minimax_h3_video_vae_fp16.safetensors | vae |
| H3 audio VAE | minimax_h3_audio_vae_fp32.safetensors | vae |

All four: https://huggingface.co/Comfy-Org/MiniMax-H3
ComfyUI offers to download them when you open the workflow.

## Upscale (optional)
The blue group needs the free custom node pack ComfyUI-SeedVR2_VideoUpscaler:
https://github.com/numz/ComfyUI-SeedVR2_VideoUpscaler (install it with ComfyUI Manager).
Its model files download by themselves on first use. No upscale wanted? Select the blue nodes and press Ctrl+B.

## Settings in the workflow
- Size 736 x 416, 20 steps, seed 4242, length 957 frames (40 seconds).
- The Length helper note has a table: 5 s = 124, 10 s = 243, 20 s = 481, 30 s = 719, 40 s = 957.
- Speech: about 2.5 words a second, so 40 seconds needs about 100 words.

The model terms of each provider apply. No model files are inside this download.
