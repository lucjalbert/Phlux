# Phlux
Phlux is a render engine learning project designed to eventually support importing models, creating lighting setups and produce visually pleasing images.
## Features implemented so far
- Application -> window -> layer system with decoupled render and update loops
- Reading position and texture coords from hardcoded vertex arrays
- Texture class with various settings and loading functions
- Shader class with runtime compilation/linking of shader programs 
- Input system for keyboard and mouse
- Fly camera class with keyboard movement and mouse based Euler angles rotation
- Ability to load .obj models with ImGui file browser
- Simple actor and scene system
- Outliner UI system
- Basic GLSL shaders
## Next up
- Basic UI to add primitives
- Simple lighting

# Primary Resources Used
- https://learnopengl.com/
- https://github.com/TheCherno/Architecture

# Trivia
- The name Phlux comes from flux, a continuous stream refering to the render/update loop of the program, and Photon, a particle of light which is the foundation of physically based rendering.
- No generative AI tools such as Claude Code or Codex were used in the making of this project. Not because I don't think they are useful tools, but rather because I find that thinking through problems myself in personal projects helps keep my mind sharp. Plus I find more fulfillment in writing code myself as well.
