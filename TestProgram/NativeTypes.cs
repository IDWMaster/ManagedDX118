using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace TestProgram
{
    struct Vector3
    {
        public float X, Y, Z;
        public static int Size
        {
            get
            {
                return sizeof(float) * 3;
            }
        }
        public Vector3(float x, float y, float z)
        {
            X = x;
            Y = y;
            Z = z;
        }
    }
    struct Vector2
    {
        public float X, Y;
        public static int Size
        {
            get
            {
                return sizeof(float) * 2;
            }
        }
        public Vector2(float x, float y)
        {
            X = x;
            Y = y;
        }
    }
    class VertexPositionNormalTexture
    {
        public static byte[] Serialize(VertexPositionNormalTexture[] iput)
        {
            MemoryStream mstream = new MemoryStream();
            BinaryWriter mwriter = new BinaryWriter(mstream);
            foreach (VertexPositionNormalTexture input in iput)
            {
                mwriter.Write(input.Vertex.X);
                mwriter.Write(input.Vertex.Y);
                mwriter.Write(input.Vertex.Z);
                mwriter.Write(input.Normal.X);
                mwriter.Write(input.Normal.Y);
                mwriter.Write(input.Normal.Z);
                mwriter.Write(input.Texture.X);
                mwriter.Write(input.Texture.Y);
            }
            mstream.Position = 0;
            byte[] buffer = new byte[mstream.Length];
            mstream.Read(buffer, 0, buffer.Length);
            
            return buffer;
        }
        public Vector3 Vertex;
        public Vector3 Normal;
        public Vector2 Texture;
        public VertexPositionNormalTexture(Vector3 vt, Vector3 nm, Vector2 tc)
        {
            Vertex = vt;
            Normal = nm;
            Texture = tc;
        }
        public static int Size
        {
            get
            {
                return Vector3.Size + Vector3.Size + Vector2.Size;
            }
        }
    }
}
