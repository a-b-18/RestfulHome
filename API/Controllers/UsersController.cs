using API.Data;
using API.DTOs;
using API.Entities;
using API.Interfaces;
using Microsoft.AspNetCore.Authorization;
using Microsoft.AspNetCore.Mvc;
using Microsoft.EntityFrameworkCore;

namespace API.Controllers
{
    public class UsersController : BaseApiController
    {
        private readonly DataContext _context;
		private readonly IUserRepository _userRepository;

		public UsersController(DataContext context, IUserRepository userRepository)
        {
            _context = context;
			_userRepository = userRepository;
		}

        [HttpGet]
        public async Task <IEnumerable<UserDto>> GetUsers()
        {
            return await _userRepository.GetUsers();
        }

        [HttpGet("{username}")]
        public async Task <ActionResult<UserDto>> GetUser(string username)
        {
            return await _userRepository.GetUser(username);
        }    
    }
}