`boost::asio::post` posts a given functor into the execution queue and that functor is executed along with other functors and completion handlers in a general way. The function is thread-safe so feel free to post from different threads.


# How

Host HTTP server via Boost.Beast, keep thread * 2 code, allocate to server.

We then need to work on grabbing data via HTTP parameters, starting with a heartbeat PoC.

