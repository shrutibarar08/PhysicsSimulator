# Project TODO List

## Completed Tasks

### Core Features
| #  | Task                                      | Description |
|----|------------------------------------------|-------------|
| 1  | Timer Class                              | Created a reusable timer for tracking elapsed time. |
| 2  | Test Timer                               | Verified the timer class functions correctly. |
| 3  | Render Primitives                        | Ensured basic shapes like cubes and lines render properly. |
| 4  | Texture Mapping                          | Applied and tested simple textures on objects. |
| 5  | Lighting System                          | Implemented and validated basic lighting effects. |
| 6  | Dynamic Object Creation                  | Designed a scalable system for dynamic objects. |
| 7  | ImGUI Integration                        | Integrated ImGUI for UI-based debugging and controls. |
| 8  | Scenario System                          | Implemented a system to manage different scenarios. |
| 9  | Shadow Mapping                           | Added basic shadows for improved realism. |
| 10 | Widget Manager                           | Created a system for dynamic UI widget rendering. |
| 11 | Scenario Manager                         | Extended scenario handling with a dedicated manager. |
| 12 | More 3D Shapes                           | Added support for spheres, capsules, and more. |
| 13 | Additional Scenarios                     | Designed extra test scenarios for engine features. |

### Scenario System
- Built scenario loading from JSON. ✅
- Tested scenario functionality. ✅
- Offloaded scenario data to JSON. ✅
- Created DisplayControl for every object. ✅
- Displayed and tested objects. ✅
- Replaced `SubsystemInterface` with an INI file for saving and loading states.

---

## Light Implementation Ideas

### Completed Tasks
- Updated input layout description. ✅
- Added normal and texture coordinate members to the vertex struct. ✅
- Constructed normals and texture coordinates for primitives. ✅
- Updated vertex and pixel shaders for testing (while keeping color). ✅

### Planned Enhancements
1. **Construct Light Class**
   - Construct buffer for pixel shader using structured binding.
   - Update the structure buffer for primitives on apply.
   - Store all lights in an array with primitives as references for efficient updates.
2. **Add Light Object for Testing**
   - Add a default light at the center for testing.
   - Ensure the scenario system supports light state behavior.

---

## Additional Completed Tasks
- Created a common manager class for lights and objects. ✅
- Implemented mouse freelook. ✅
- Developed a control panel GUI. ✅
- Created GUI controls for objects. ✅
- Designed GUI controls for lights. ✅
- Established a common structure for all light types. ✅
- Developed separate GUI interfaces for different light types. ✅

---

## TODO
- **Refactor GUI for Objects** ✅
  - Assign unique names for distinction.
- **Extend GUI for Light Management** ✅
- **Improve JSON State Management** ✅
  - Enhance functionality for saving and restoring states. 
- **Optimize Vertex Shader and Vertex Data** ✅
- **Support Multiple Object Types** ✅

---

## Notes
- The current implementation is functional, but further refinements are needed.
- Focus on optimizing the light system and GUI interactions next.

**[This structure should work efficiently moving forward.]**

