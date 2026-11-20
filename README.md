# AI Waifu Pipeline: Identity Genesis & Dataset Generation

Pipeline for creating a unique character with absolute face identity retention and automated dataset generation for LoRA training.

## Repository Structure

* `ComfyUI-Shared/workflows/`
    * `pt1_create_portrait/` — Creating the base identity (face genesis).
        * `wf1_create_body_frame.json` — Step 1: Generating the base pose and body frame.
        * `wf2_make_face_genesis.json` — Step 2: Locking in facial features.
        * `wf3_retouch.json` — Step 3: Final upscale, artifact cleanup.
    * `pt2_create_dataset/` — Mass dataset generation for LoRA.
        * `ai_syndicate_dataset_creator_v3.json` — Workflow for automated dataset creation (version 3).
        * `Consistent Character dataset 9b (ver 1.0.1).json` — Workflow for maintaining character consistency during dataset generation.
        * `flux_dataset_prompts.txt` — Prompt set optimized for the Flux T5 encoder.
        * `for_dataset_sfw.txt` — Base of unique SFW frames (angles, clothing, locations).
    * `pt3_utils/` — Additional tools for inpainting, editing, and Image-to-Image.
        * `Flux 2 Klein Inpaint Workflow.json` — Targeted inpainting based on Flux 2 Klein models.
        * `Flux2 Klein 9B Distill Editing 1.0.json` — Editing individual character elements and attributes via distilled models.
        * `Flux2Klein.json` — Base generation and customization pipeline for the Klein architecture.
        * `Krea2_Reference_I2I_v1.json` — Image-to-Image reference transfer in the Krea2 style.
        * `krea2InpaintClickTo_v13.json` — Fast click/mask inpainting for correcting minor flaws and artifacts.

## 🧠 Methodology: Modular Prompt Engineering (Krea2)

To achieve maximum dataset diversity and prevent overfitting on specific scenes, a **matrix approach** is used. Instead of static prompt lists, we create a dynamic system.

### Concept:
1.  **`%MAIN_PROMPT%` (The Biological Constant):** Shot-agnostic identity "anchor." Contains 100% of the character's permanent physical features (face, eyes, hair, skin, piercings, body type). **Does not contain** descriptions of clothing, lighting, or location.
2.  **Variable Modifiers (Modular Blocks):** Parameter sets that combine with `%MAIN_PROMPT%` to create unique frames.

### Prompt Category Matrix:
*   **Technical/Angles:** Angles and framing (Front view, Profile, Close-up).
*   **Lifestyle/Casual:** Everyday scenarios, soft natural lighting.
*   **High-end Editorial:** Cinematic lighting, complex textures, professional photography.
*   **Headshots/Close-ups:** Focus on micro-facial details and skin texture.
*   **Action & Dynamics:** Dynamic poses, sports, high facial expression load.
*   **Sensual & Intimate:** Work with soft fabrics (silk, lace) and intimate lighting.
*   **Mixed/Moods:** Mixed archetypes for maximum variability coverage.

---

## How to Run

### Phase 1: Character Creation (Identity Genesis)
1. Open ComfyUI and sequentially run the workflows from the `pt1_create_portrait` folder (steps `wf1` → `wf2` → `wf3`).
2. The output will be a high-definition reference portrait of the character, capturing the appearance.

### Phase 2: Dataset Generation (Row-based Batching)
Use one of the workflows in the `pt2_create_dataset` folder. The pipeline operates on a **Modular Matrix**:

1. Load the selected JSON workflow into ComfyUI.
2. Connect the text lists (generated according to the modular matrix) to the prompt parser node.
3. Configure the KSampler for **row-based batching**. The system will take `%MAIN_PROMPT%` and dynamically append variable modifiers from the current batch, creating a unique frame per frame.

### Phase 3: Post-processing and Editing (Optional Tools)
If you need to fix details or change the style on frames:
1. Use workflows from the `pt3_utils` folder for targeted work.

### Phase 4: Verification and QA (Validation Protocol)
To control quality and ensure 100% character consistency, every generated frame must pass the verification protocol:
`@docs/agent/validator_gen_images.md`

Verification includes checking all biological constants (piercings, moles, anatomical features) against the Master Prompt. Any deviations (identity drift) are considered defects and must be removed from the dataset.

---

## 🚀 TODO / Next Pipeline Phases

*   **Integration with Flux-Gym / Kohya_ss** — step-by-step presets and configs for quick LoRA training startup for Flux and SDXL architectures.
*   **Automatic Tagging (Captioning)** — integration of WD14 Tagger / Florence-2 nodes to generate text descriptions for each filtered frame before training.
*   **Batch Model Testing** — workflow for automatically running the trained LoRA against a test grid of prompts (XYZ plot) to evaluate identity retention across different checkpoints.
