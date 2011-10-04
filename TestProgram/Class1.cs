using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using ManagedDirectX;
using Windows.Storage;
using Windows.Storage.Streams;
namespace TestProgram
{
    public sealed class EntryPoint
    {
        public void NtfyExecutionAbrt()
        {
        }
        
        public EntryPoint()
        {
            
            beginexecblock();
        }
        float rotation = 0;
        void onrenderframe()
        {
            if(vertcount>0) {
                if (defaultmatrix != null)
                {
                    defaultmatrix.SetModelRotation(0, rotation, 0);
                }
            maincontext.Draw(vertcount);
                }
            rotation -= 0.4f;

        }
        DXMatrix defaultmatrix;
        int vertcount = 0;
        Shader defaultshader;
        RenderContext maincontext;
        async void beginexecblock()
        {
            
            
            
                RenderContext mtext = new RenderContext();
                maincontext = mtext;
                StorageFolder folder = Windows.ApplicationModel.Package.Current.InstalledLocation;
                StorageFile file = await folder.GetFileAsync("DXInteropLib\\VertexShader.cso");
                
                var stream = (await file.OpenAsync(FileAccessMode.Read));
                Windows.Storage.Streams.DataReader mreader = new Windows.Storage.Streams.DataReader(stream.GetInputStreamAt(0));
                byte[] dgram = new byte[file.Size];
                await mreader.LoadAsync((uint)dgram.Length);
                mreader.ReadBytes(dgram);
                file = await folder.GetFileAsync("DXInteropLib\\PixelShader.cso");

                stream = await file.OpenAsync(FileAccessMode.Read);
                mreader = new Windows.Storage.Streams.DataReader(stream.GetInputStreamAt(0));
                byte[] mgram = new byte[file.Size];
                await mreader.LoadAsync((uint)file.Size);
                mreader.ReadBytes(mgram);
                
                    defaultshader = mtext.CreateShader(dgram, mgram);
                    mtext.InitializeLayout(dgram);
                    defaultshader.Apply();
                    mtext.OnRenderFrame += onrenderframe;
                
                IStorageFile[] files = (await folder.GetFilesAsync()).ToArray();
                bool founddata = false;
                foreach (IStorageFile et in files)
                {
                    if (et.FileName.Contains("rawimage.dat"))
                    {
                        stream = await et.OpenAsync(FileAccessMode.Read);
                        founddata = true;
                    }
                }
                int width;
                int height;
                byte[] rawdata;
                if (!founddata)
                {
                    file = await folder.GetFileAsync("TestProgram\\test.jpg");
                    stream = await file.OpenAsync(FileAccessMode.Read);
                    var decoder = await Windows.Graphics.Imaging.BitmapDecoder.CreateAsync(stream);
                    var pixeldata = await decoder.GetPixelDataAsync(Windows.Graphics.Imaging.BitmapPixelFormat.Rgba8, Windows.Graphics.Imaging.BitmapAlphaMode.Straight, new Windows.Graphics.Imaging.BitmapTransform(), Windows.Graphics.Imaging.ExifOrientationMode.IgnoreExifOrientation, Windows.Graphics.Imaging.ColorManagementMode.DoNotColorManage);
                    width = (int)decoder.PixelWidth;
                    height = (int)decoder.PixelHeight;

                    rawdata = pixeldata.DetachPixelData();
                    file = await folder.CreateFileAsync("rawimage.dat");
                    stream = (await file.OpenAsync(FileAccessMode.ReadWrite));
                    var realstream = stream.GetOutputStreamAt(0);
                    DataWriter mwriter = new DataWriter(realstream);
                    mwriter.WriteInt32(width);
                    mwriter.WriteInt32(height);
                    mwriter.WriteBytes(rawdata);
                    await mwriter.StoreAsync();
                    await realstream.FlushAsync();
                    
                    
                    
                }
                else
                {
                    DataReader treader = new DataReader(stream.GetInputStreamAt(0));
                    await treader.LoadAsync((uint)stream.Size);
                    rawdata = new byte[stream.Size-(sizeof(int)*2)];
                    width = treader.ReadInt32();
                    height = treader.ReadInt32();
                    treader.ReadBytes(rawdata);
                }
                Texture2D mtex = maincontext.createTexture2D(rawdata, width, height);
                mtex.Draw();
                #region Cube
                List<VertexPositionNormalTexture> triangle = new List<VertexPositionNormalTexture>();
                float z = 0;
               
                triangle.Add(new VertexPositionNormalTexture(new Vector3(0,0,z),new Vector3(1,1,1),new Vector2(0,0)));
                triangle.Add(new VertexPositionNormalTexture(new Vector3(1,1,z),new Vector3(1,1,1),new Vector2(1,1)));
                triangle.Add(new VertexPositionNormalTexture(new Vector3(0,1,z),new Vector3(1,1,1),new Vector2(0,1)));
                //Triangle 2
                triangle.Add(new VertexPositionNormalTexture(new Vector3(0, 0, z), new Vector3(1, 1, 1), new Vector2(0, 0)));
                triangle.Add(new VertexPositionNormalTexture(new Vector3(1,0,z),new Vector3(1,1,1),new Vector2(1,0)));
                triangle.Add(new VertexPositionNormalTexture(new Vector3(1, 1, z), new Vector3(1, 1, 1), new Vector2(1, 1)));
               // Triangle 3
                triangle.Add(new VertexPositionNormalTexture(new Vector3(1, 0, z),new Vector3(1,1,1),new Vector2(0,0)));
                triangle.Add(new VertexPositionNormalTexture(new Vector3(1, 1, z+1), new Vector3(1, 1, 1), new Vector2(1, 1)));
                triangle.Add(new VertexPositionNormalTexture(new Vector3(1, 0, z + 1), new Vector3(1, 1, 1), new Vector2(0, 1)));
                //Triangle 4
                triangle.Add(new VertexPositionNormalTexture(new Vector3(1, 0, z), new Vector3(1, 1, 1), new Vector2(0, 0)));
                triangle.Add(new VertexPositionNormalTexture(new Vector3(1, 1, z), new Vector3(1, 1, 1), new Vector2(1, 0)));
                triangle.Add(new VertexPositionNormalTexture(new Vector3(1, 1, z + 1), new Vector3(1, 1, 1), new Vector2(1, 1)));
                
                //Triangle 5
                triangle.Add(new VertexPositionNormalTexture(new Vector3(0, 0, z), new Vector3(1, 1, 1), new Vector2(0, 0)));
                triangle.Add(new VertexPositionNormalTexture(new Vector3(0, 1, z + 1), new Vector3(1, 1, 1), new Vector2(1, 1)));
                triangle.Add(new VertexPositionNormalTexture(new Vector3(0, 0, z + 1), new Vector3(1, 1, 1), new Vector2(0, 1)));
                //Triangle 6
                triangle.Add(new VertexPositionNormalTexture(new Vector3(0, 0, z), new Vector3(1, 1, 1), new Vector2(0, 0)));
                triangle.Add(new VertexPositionNormalTexture(new Vector3(0, 1, z), new Vector3(1, 1, 1), new Vector2(1, 0)));
                triangle.Add(new VertexPositionNormalTexture(new Vector3(0, 1, z + 1), new Vector3(1, 1, 1), new Vector2(1, 1)));
                //Triangle 7

                triangle.Add(new VertexPositionNormalTexture(new Vector3(0, 0, z+1), new Vector3(1, 1, 1), new Vector2(0, 0)));
                triangle.Add(new VertexPositionNormalTexture(new Vector3(1, 1, z+1), new Vector3(1, 1, 1), new Vector2(1, 1)));
                triangle.Add(new VertexPositionNormalTexture(new Vector3(0, 1, z+1), new Vector3(1, 1, 1), new Vector2(0, 1)));
                //Triangle 8
                triangle.Add(new VertexPositionNormalTexture(new Vector3(0, 0, z+1), new Vector3(1, 1, 1), new Vector2(0, 0)));
                triangle.Add(new VertexPositionNormalTexture(new Vector3(1, 0, z+1), new Vector3(1, 1, 1), new Vector2(1, 0)));
                triangle.Add(new VertexPositionNormalTexture(new Vector3(1, 1, z+1), new Vector3(1, 1, 1), new Vector2(1, 1)));
                //Top face
                //Triangle 9
                //0,0
                triangle.Add(new VertexPositionNormalTexture(new Vector3(0, 1, 0), new Vector3(1, 0, 1), new Vector2(0, 0)));
                //1,1
                triangle.Add(new VertexPositionNormalTexture(new Vector3(1, 1, 1), new Vector3(1, 0, 0), new Vector2(1, 1)));
                //0,1
                triangle.Add(new VertexPositionNormalTexture(new Vector3(0, 1, 1), new Vector3(0, 1, 1), new Vector2(0, 1)));
                //Triangle 10
                //0,0
                triangle.Add(new VertexPositionNormalTexture(new Vector3(0, 1, 0), new Vector3(1, 1, 1), new Vector2(0, 0)));
          
                //1,0
                triangle.Add(new VertexPositionNormalTexture(new Vector3(1, 1, 0), new Vector3(1, 1, 1), new Vector2(1, 0)));
                
                //1,1
                triangle.Add(new VertexPositionNormalTexture(new Vector3(1, 1, 1), new Vector3(1, 1, 1), new Vector2(1, 1)));
                #endregion
                byte[] gpudata = VertexPositionNormalTexture.Serialize(triangle.ToArray());
                
                VertexBuffer mbuffer = maincontext.createVertexBuffer(gpudata,VertexPositionNormalTexture.Size);
                mbuffer.Apply(VertexPositionNormalTexture.Size);
                vertcount = triangle.Count;
                
                defaultmatrix = maincontext.createMatrix(true);
                defaultmatrix.SetCameraProperties(new Vector3D(0, 2, -1.5f), new Vector3D(0, 0, 0));
                defaultmatrix.Activate(0);
            
        }

    }
}
