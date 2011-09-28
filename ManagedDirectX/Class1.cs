using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using DXInteropLib;
namespace ManagedDirectX
{
    public sealed class Texture2D
    {
        IntPtr internhandle;
        /// <summary>
        /// Maps the current texture to the shader
        /// </summary>
        public void Draw()
        {
            unsafe
            {
                itext.underlyingcontext.ApplyTexture(internhandle.ToPointer());
            }
        }
        RenderContext itext;
        internal Texture2D(IntPtr handle, RenderContext context)
        {
            itext = context;
            internhandle = handle;
            if (handle == IntPtr.Zero)
            {
                throw new Exception("An error occured while creating the texture. You may have passed in some invalid bytecode, or a bad width/height");
            }
        }
    }
    public sealed class VertexBuffer
    {
        IntPtr internhandle;
        RenderContext interntext;
        internal VertexBuffer(IntPtr handle, RenderContext context)
        {
            internhandle = handle;
            interntext = context;
            if (handle == IntPtr.Zero)
            {
                throw new Exception("An error occured while creating the vertex buffer. Perhaps you forgot to purchase a GPU for your ancient computer.");
            }
        }
        public void Apply(int stride)
        {
            unsafe {
                interntext.underlyingcontext.BindVertexBuffer(internhandle.ToPointer(),stride);
            }
        }
    }
    public sealed class Shader
    {
        IntPtr internhandle;
        RenderContext interntext;
        internal Shader(IntPtr handle, RenderContext context)
        {
            if (handle == IntPtr.Zero)
            {
                throw new Exception("An error occured while creating the shader object. Make sure your code was compiled correctly.");
            }
            internhandle = handle;
            interntext = context;
        }
        /// <summary>
        /// Applies the shader to the current device context
        /// </summary>
        public void Apply()
        {
            unsafe
            {
                interntext.underlyingcontext.ApplyVertexShader(internhandle.ToPointer());
            }
        }
    }
    public sealed class RenderContext
    {
        internal DirectContext underlyingcontext;
        public RenderContext()
        {
            underlyingcontext = DirectContext.getDefaultContext();
            underlyingcontext.RegisterLoopHandler(loophandler);
        }
        public event RenderLoopArgs OnRenderFrame;
        void loophandler()
        {
            if (OnRenderFrame != null)
            {
                OnRenderFrame.Invoke();
            }
        }
        /// <summary>
        /// Initializes the underlying DirectX layout (really wish this was more like OpenGL....)
        /// </summary>
        /// <param name="vertshader"></param>
        public void InitializeLayout(byte[] vertshader)
        {
            underlyingcontext.InitializeInputLayout(vertshader);
        }
        /// <summary>
        /// Creates a vertex buffer
        /// </summary>
        /// <param name="data">The data to load into the vertex buffer</param>
        /// <param name="scan">The byte width of each Vertex</param>
        /// <returns></returns>
        public VertexBuffer createVertexBuffer(byte[] data, int scan)
        {
            unsafe
            {
                return new VertexBuffer(new IntPtr(underlyingcontext.CreateVertexBuffer(data, scan)),this);
            }
        }
        /// <summary>
        /// Creates a 32-bit 2D texture
        /// </summary>
        /// <param name="data">The raw data</param>
        /// <param name="width">The width</param>
        /// <param name="height">The height</param>
        /// <returns></returns>
        public Texture2D createTexture2D(byte[] data, int width, int height)
        {
            unsafe
            {
                return new Texture2D(new IntPtr(underlyingcontext.CreateTexture2D(data, width, height)),this);
            }
        }
        /// <summary>
        /// Draws the specified number of vertices
        /// </summary>
        /// <param name="vertcount"></param>
        public void Draw(int vertcount)
        {
            underlyingcontext.Draw(vertcount);
        }
        public Shader CreateShader(byte[] vertexfile, byte[] pixelfile)
        {
            unsafe
            {
                return new Shader(new IntPtr(underlyingcontext.CreateVertexShader(vertexfile, pixelfile)),this);
            }
        }
        public void dosomething()
        {
        }
    }
}
