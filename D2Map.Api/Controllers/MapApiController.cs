using D2Map.Api.Controllers.Models;
using D2Map.Core;
using Microsoft.AspNetCore.Mvc;

namespace D2Map.Api.Controllers
{
    [ApiController]
    [Route("/maps")]
    public class MapApiController : ControllerBase
    {
        private readonly IMapService _mapService;

        public MapApiController(IMapService mapService)
        {
            _mapService = mapService;
        }

        [HttpGet]
        public IActionResult Get([FromQuery] MapParameters parameters)
        {
           return Ok(_mapService.GetCollisionMap(parameters.MapId, parameters.Difficulty.GetValueOrDefault(), parameters.Area.GetValueOrDefault()));
        }
    }
}
