//
// Created by Tom Smale on 10/01/2025.
//

#include <raylib.h>
#include <array>

enum class Voxel : uint16_t {
    kAir = 0,
    kDirt,
    kGrass,
    kStone,
    kSand,
    kWater,
    kCount
};

static constexpr Color kVoxelColors[] = {
    BLANK, // Air
    BROWN, // Dirt
    GREEN, // Grass
    GRAY,  // Stone
    BEIGE,// Sand
    BLUE,  // Water
};

class Chunk {
 public:
  static constexpr int kWidth = 16;
  static constexpr int kHeight = 16;
  static constexpr int kSize = kWidth * kWidth * kHeight;

  Chunk () {
    voxels_ = new Voxel[kSize];
    Fill(Voxel::kAir);
  }

  ~Chunk() {
    delete[] voxels_;
  }

  Voxel GetVoxel(int index) const {
    return voxels_[index];
  }

  Voxel GetVoxel(int x, int y, int z) const {
    return voxels_[x + z*kWidth + y*kWidth*kHeight];
  }

  void SetVoxel(int index, Voxel voxel) {
    voxels_[index] = voxel;
  }

  void SetVoxel(int x, int y, int z, Voxel voxel) {
    voxels_[x + z*kWidth + y*kWidth*kHeight] = voxel;
  }

  void Fill(Voxel voxel) {
    std::memset(voxels_, static_cast<uint16_t>(voxel), kSize * sizeof(Voxel));
  }

  /*void FillBox(int x1, int y1, int z1, int x2, int y2, int z2, Voxel voxel) {
    for (int y = y1; y <= y2; ++y) { // Iterate over the y dimension
      for (int z = z1; z <= z2; ++z) { // Iterate over the z dimension
        // Calculate the starting index for the strip of contiguous x elements
        // x1 + z * kWidth + y * kWidth * kHeight:
        // - x1 is the starting x position
        // - z * kWidth moves to the correct z slice
        // - y * kWidth * kHeight moves to the correct y slice
        std::memset(&voxels_[x1 + z * kWidth + y * kWidth * kHeight],
                    static_cast<uint16_t>(voxel), // Convert Voxel to its underlying integer type
                    (x2 - x1 + 1) * sizeof(Voxel)); // Number of bytes to set
      }
    }
  }*/

  void FillBox(int x1, int y1, int z1, int x2, int y2, int z2, Voxel voxel) {
    for (int y = y1; y <= y2; ++y) {
        for (int z = z1; z <= z2; ++z) {
            for (int x = x1; x <= x2; ++x) {
                voxels_[x + z * kWidth + y * kWidth * kHeight] = voxel;
            }
        }
    }
}

  void FillBox(Vector3 pos1, Vector3 pos2, Voxel voxel) {
    FillBox(static_cast<int>(pos1.x), static_cast<int>(pos1.y), static_cast<int>(pos1.z),
            static_cast<int>(pos2.x), static_cast<int>(pos2.y), static_cast<int>(pos2.z),
            voxel);
  }

  void Draw() {
    for (int y = 0; y < kHeight; ++y) {
      for (int z = 0; z < kWidth; ++z) {
        for (int x = 0; x < kWidth; ++x) {
          Voxel voxel = GetVoxel(x, y, z);
          if (voxel != Voxel::kAir) {
            DrawCube({(float)x-kWidth/2, (float)y-kHeight/2, (float)z-kWidth/2},
                     1.0f, 1.0f, 1.0f,
                     kVoxelColors[static_cast<int>(voxel)]);
          }
        }
      }
    }
  }

 private:
  Voxel* voxels_;
};

int main() {
  InitWindow(800, 450, "Hello, Raylib!");
  SetTargetFPS(60);

  Chunk chunk;
  chunk.FillBox({0, 0, 0},
                {Chunk::kWidth-1, (Chunk::kHeight-1)/2, Chunk::kWidth-1},
                Voxel::kDirt);
  chunk.FillBox({0, (Chunk::kHeight-1)/2, 0},
                {Chunk::kWidth-1, (Chunk::kHeight-1)/2, Chunk::kWidth-1},
                Voxel::kGrass);

  Camera camera = {.position   = {4.0f, 4.0f, 4.0f},
                   .target     = {0.0f, 0.0f, 0.0f},
                   .up         = {0.0f, 1.0f, 0.0f},
                   .fovy       = 60.0f,
                   .projection = CAMERA_PERSPECTIVE};

  while (!WindowShouldClose()) {
    UpdateCamera(&camera, CAMERA_FREE);

    BeginDrawing();
    ClearBackground(RAYWHITE);
    BeginMode3D(camera);
    chunk.Draw();
    EndMode3D();
    DrawFPS(10, 10);
    EndDrawing();
  }

  CloseWindow();
  return 0;
}