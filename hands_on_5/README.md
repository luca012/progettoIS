# Main changes to AuthController
- Implemented the `@PostMapping("/login")` endpoint to verify credentials.
- Added input validation for null or blank usernames and passwords during login.
- Improved HTTP response handling with appropriate status codes (BAD_REQUEST, UNAUTHORIZED, OK).
