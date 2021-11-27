using System.Collections.Generic;
using System.Threading.Tasks;
using API.DTOs;
using API.Entities;

namespace API.Interfaces
{
    public interface IUserRepository
    {
        Task<bool> SaveAllAsync();
        Task<UserDto> GetUser(string username);
        Task<IEnumerable<UserDto>> GetUsers();
    }
}