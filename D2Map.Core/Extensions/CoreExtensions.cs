using D2Map.Core.Wrapper;
using Microsoft.Extensions.Configuration;
using Microsoft.Extensions.DependencyInjection;
using System.IO;

namespace D2Map.Core.Extensions
{
    public static class CoreExtensions
    {
        public static void RegisterCoreServices(this IServiceCollection services, IConfiguration config)
        {
            services.AddSingleton<IMapService, MapService>();
            if(!Directory.Exists(config["Diablo2Directory"]))
            {
                throw new System.Exception($"Provided invalid diablo 2 directory: {config["Diablo2Directory"]}");
            }
            MapDll.Initialize(config["Diablo2Directory"]);
        }
    }
}
