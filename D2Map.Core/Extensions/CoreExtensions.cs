using D2Map.Core.Wrapper;
using Microsoft.Extensions.Configuration;
using Microsoft.Extensions.DependencyInjection;
using System;
using System.IO;

namespace D2Map.Core.Extensions
{
    public static class CoreExtensions
    {
        public static void RegisterCoreServices(this IServiceCollection services, IConfiguration config)
        {
            services.AddSingleton<IMapService, MapService>();
            var directory = config["Diablo2Directory"];
            if (!Directory.Exists(directory))
            {
                throw new Exception($"Provided invalid diablo 2 directory: {directory}");
            }

            if (!MapDll.Initialize(directory))
            {
                var version = (GameVersion)MapDll.GetGameVersion();
                var timestamp = MapDll.GetD2CommonTimestamp();
                if (timestamp == 0)
                {
                    throw new Exception($"Could not load D2Common.dll from {directory}: is this a Diablo II installation?");
                }
                if (version == GameVersion.Unknown)
                {
                    throw new Exception($"Unsupported Diablo II version in {directory} (D2Common.dll timestamp 0x{timestamp:X8}). Supported versions: 1.13c, 1.09d.");
                }
                throw new Exception($"Failed to initialize Diablo II {version} from {directory} (error {MapDll.GetLastError()}).");
            }

            var detected = (GameVersion)MapDll.GetGameVersion();
            D2Offsets.Select(detected);
            Console.WriteLine($"d2mapapi: initialized Diablo II {detected} from {directory}");
        }
    }
}
