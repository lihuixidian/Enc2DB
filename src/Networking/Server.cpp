#include "Server.h"
#include "config.h"

using namespace util;

Server::Server(boost::asio::io_service& io_service, int port) : io_service_(io_service), acceptor_(io_service,
            boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)),
    context_(boost::asio::ssl::context::sslv23) {

    this->context_.set_options(boost::asio::ssl::context::default_workarounds
                               | boost::asio::ssl::context::no_sslv2
                               | boost::asio::ssl::context::single_dh_use);

    this->context_.use_certificate_file(Config::getInstance()->Get("RA.serverCrt", "").asString(), boost::asio::ssl::context::pem);
    // .use_certificate_chain_file(Config::getInstance()->Get("RA.serverKey", "").asString());
    this->context_.use_private_key_file(Config::getInstance()->Get("RA.serverKey", "").asString(), boost::asio::ssl::context::pem);

    Log("Certificate \"" + Config::getInstance()->Get("RA.serverKey", "").asString() + "\" set");
    Log("Server running on port: %d", port);
}


Server::~Server() {}


void Server::start_accept() {
    Session *new_session = new Session(io_service_, context_);
    new_session->setCallbackHandler(this->callback_handler);
    acceptor_.async_accept(new_session->socket(), boost::bind(&Server::handle_accept, this, new_session, boost::asio::placeholders::error));
}

void Server::handle_accept(Session* new_session, const boost::system::error_code& error) {
    if (!error) {
        Log("New accept request, starting new session");
        new_session->start();
    } else {
        delete new_session;
    }

    start_accept();
}

void Server::connectCallbackHandler(CallbackHandler cb) {
    this->callback_handler = cb;
}








