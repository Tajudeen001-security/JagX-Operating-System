# JagX Compositor (Early Design)

This is the beginning of the window system / compositor that will serve both PC and Mobile.

## Goals

- Shared core concepts for desktop and mobile
- Low latency (especially important for mobile touch)
- Support for the dark + teal/purple visual language
- Smooth animations and damage tracking later

## Planned Responsibilities

- Window list and stacking
- Surface / buffer management
- Input routing (pointer, touch, keyboard)
- Composition of the final image
- Output management (displays, scaling)

## Current Code

Very early C structures only. Real rendering comes after we have a working framebuffer.
