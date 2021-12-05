using D2Map.Core.Models;
using Microsoft.AspNetCore.Mvc;
using System.ComponentModel.DataAnnotations;

namespace D2Map.Api.Controllers.Models
{
    public class MapParameters
    {
        [Required]
        [FromQuery]
        public uint MapId { get; set; }

        [Required]
        [FromQuery]
        public Difficulty? Difficulty { get; set; }

        [Required]
        [FromQuery]
        public Area? Area { get; set; }
    }
}
