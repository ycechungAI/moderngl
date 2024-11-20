import moderngl
import glm
import numpy as np
from PIL import Image

ctx = moderngl.create_standalone_context()

fbo = ctx.framebuffer([ctx.renderbuffer((800, 800), components=4, samples=16)])
output = ctx.framebuffer([ctx.renderbuffer((800, 800), components=4)])

prog = ctx.program(
    task_shader="""
        #version 460

        #extension GL_NV_mesh_shader : require
        #extension GL_NV_shader_thread_group : require
        #extension GL_NV_gpu_shader5 : require

        layout(local_size_x = 6) in;

        layout(std430, binding = 0) buffer inputBlockData {
            uint block_types[32 * 32 * 32];
        };

        // faces:
        // 0 = DOWN
        // 1 = UP
        // 2 = NORTH
        // 3 = SOUTH
        // 4 = WEST
        // 5 = EAST

        const int faceBlockArrayOffsets[] = {-32, 32, -1024, 1024, -1, 1};
        const ivec3 faceBlockArrayDirections[] = {
            ivec3(0, -1, 0),
            ivec3(0, 1, 0),
            ivec3(0, 0, -1),
            ivec3(0, 0, 1),
            ivec3(-1, 0, 0),
            ivec3(1, 0, 0)
        };

        taskNV out Task {
            uint      block_position;
            uint8_t   faceIds[6];
        } OUT;

        bool checkCullFace(int position, uint face) {
            int offset = faceBlockArrayOffsets[face];
            int x = position % 32;
            int y = (position / 32) % 32;
            int z = position / 1024;
            ivec3 vpos = ivec3(x, y, z);
            ivec3 adjPos = vpos + faceBlockArrayDirections[face];
            return any(lessThan(adjPos, ivec3(0)))
                || any(greaterThanEqual(adjPos, ivec3(16)))
                || block_types[offset + position] == 0;
        }

        void main() {
            int position = int(gl_WorkGroupID.x);
            uint block_type = block_types[position];
            bool isFaceVisible = block_type != 0 && checkCullFace(position, gl_LocalInvocationID.x);
            uint vote = ballotThreadNV(isFaceVisible);
            uint faceCount = bitCount(vote);
            gl_TaskCountNV = faceCount;
            OUT.block_position = position;
            uint idxOffset = bitCount(vote & gl_ThreadLtMaskNV);
            if (isFaceVisible) {
                OUT.faceIds[idxOffset] = uint8_t(gl_LocalInvocationID.x);
            }
        }
    """,
    mesh_shader="""
        #version 460

        #extension GL_NV_mesh_shader : require
        #extension GL_NV_shader_thread_group : require
        #extension GL_NV_gpu_shader5 : require

        layout(local_size_x = 6) in;
        layout(triangles, max_vertices = 6, max_primitives = 2) out;
        //layout(points, max_vertices = 1, max_primitives = 1) out;

        taskNV in Task {
            uint      block_position;
            uint8_t   faceIds[6];
        } IN;

        out PerVertexData {
            vec4 color;
        } v_out[];

        const vec3 cubeFaceVertices[6][6] = {
            {
                vec3(1.0, 0.0, 0.0),
                vec3(1.0, 0.0, 1.0),
                vec3(0.0, 0.0, 1.0),
                vec3(0.0, 0.0, 1.0),
                vec3(0.0, 0.0, 0.0),
                vec3(1.0, 0.0, 0.0)
            },
            {
                vec3(0.0, 1.0, 0.0),
                vec3(0.0, 1.0, 1.0),
                vec3(1.0, 1.0, 1.0),
                vec3(1.0, 1.0, 1.0),
                vec3(1.0, 1.0, 0.0),
                vec3(0.0, 1.0, 0.0)
            },
            {
                vec3(0.0, 1.0, 0.0),
                vec3(1.0, 1.0, 0.0),
                vec3(1.0, 0.0, 0.0),
                vec3(1.0, 0.0, 0.0),
                vec3(0.0, 0.0, 0.0),
                vec3(0.0, 1.0, 0.0)
            },
            {
                vec3(0.0, 0.0, 1.0),
                vec3(1.0, 0.0, 1.0),
                vec3(1.0, 1.0, 1.0),
                vec3(1.0, 1.0, 1.0),
                vec3(0.0, 1.0, 1.0),
                vec3(0.0, 0.0, 1.0)
            },
            {
                vec3(0.0, 0.0, 0.0),
                vec3(0.0, 0.0, 1.0),
                vec3(0.0, 1.0, 1.0),
                vec3(0.0, 1.0, 1.0),
                vec3(0.0, 1.0, 0.0),
                vec3(0.0, 0.0, 0.0)
            },
            {
                vec3(1.0, 1.0, 0.0),
                vec3(1.0, 1.0, 1.0),
                vec3(1.0, 0.0, 1.0),
                vec3(1.0, 0.0, 1.0),
                vec3(1.0, 0.0, 0.0),
                vec3(1.0, 1.0, 0.0)
            }
        };

        layout(location = 0) uniform mat4 modelViewProjection;

        void main() {
            uint position = IN.block_position;
            uint8_t face = IN.faceIds[gl_WorkGroupID.x];
            uint vertexId = gl_LocalInvocationID.x;
            float x = float(position % 32);
            float y = float((position / 32) % 32);
            float z = float(position / 1024);
            vec3 blockPos = vec3(x, y, z);
            gl_MeshVerticesNV[vertexId].gl_Position = modelViewProjection * vec4(cubeFaceVertices[face][vertexId] + blockPos, 1.0);
            v_out[vertexId].color = vec4(blockPos / 32.0, 1.0);
            // gl_MeshVerticesNV[vertexId].gl_Position = modelViewProjection * vec4(blockPos, 1.0);
            gl_PrimitiveIndicesNV[vertexId] = vertexId;
            gl_PrimitiveCountNV = 2;
            // gl_PrimitiveCountNV = 1;
        }
    """,
    fragment_shader="""
        #version 460

        out vec4 fragColor;

        in PerVertexData {
            vec4 color;
        } fragIn;

        void main() {
            fragColor = fragIn.color;
        }
    """,
)

voxel_buffer = ctx.buffer(np.random.randint(0, 2, 32 * 32 * 32))
voxel_buffer.bind_to_storage_buffer(0)

proj = glm.perspective(45.0, 1.0, 0.1, 1000.0)
look = glm.lookAt((48.0, 48.0, 48.0), (0.0, 0.0, 0.0), (0.0, 0.0, 1.0))
camera = proj * look

prog["modelViewProjection"].write(camera)

fbo.use()
ctx.clear()
prog.draw_mesh_tasks(0, 32 * 32 * 32)

ctx.copy_framebuffer(output, fbo)

img = Image.frombuffer("RGBA", output.size, output.read(components=4)).transpose(
    Image.Transpose.FLIP_TOP_BOTTOM
)
img.save("out.png")
