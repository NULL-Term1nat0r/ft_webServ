

###################
#  UBUNTU VERSION
###################
# Use the official Ubuntu image
FROM ubuntu:latest

# Set noninteractive mode for apt-get
ARG DEBIAN_FRONTEND=noninteractive

# Install Zsh
RUN apt-get update && \
    apt-get install -y zsh

# Set the default shell to Zsh
SHELL ["/bin/zsh", "-c"]

# Install other required packages
RUN echo "ipv6" >> /etc/modules && \
    apt-get update && \
    apt-get upgrade -y && \
    apt-get install -y \
    build-essential \
    valgrind \
    libx11-dev \
    libxext-dev \
    libxrandr-dev \
    libxrender-dev \
    libbsd-dev \
    libffi-dev \
    liblzma-dev \
    libbz2-dev \
    clang \
    clang-format \
    cmake \
    git \
    siege \
    curl && \
    [ ! -d "/root/.oh-my-zsh" ] && sh -c "$(curl -fsSL https://raw.githubusercontent.com/ohmyzsh/ohmyzsh/master/tools/install.sh)" || true && \
    [ ! -d "/root/.oh-my-zsh/custom/plugins/zsh-syntax-highlighting" ] && git clone https://github.com/zsh-users/zsh-syntax-highlighting.git /root/.oh-my-zsh/custom/plugins/zsh-syntax-highlighting || true && \
    [ ! -d "/root/.oh-my-zsh/custom/plugins/zsh-autosuggestions" ] && git clone https://github.com/zsh-users/zsh-autosuggestions.git /root/.oh-my-zsh/custom/plugins/zsh-autosuggestions || true && \
    sed -i 's/plugins=(git)/plugins=(git z common-aliases node npm rand-quote sudo colored-man-pages colorize cp zsh-syntax-highlighting zsh-autosuggestions copyfile history dirhistory)/' /root/.zshrc && \
    ln -sf /usr/bin/clang /usr/bin/cc && \
    ln -sf /usr/bin/clang++ /usr/bin/c++

# Set the working directory
WORKDIR /app

###################
#  ALPINE VERSION
###################

# FROM alpine:latest

# RUN echo "ipv6" >> /etc/modules && \
#     apk update && \
#     apk upgrade && \
#     apk add --no-cache \
#     build-base \
#     valgrind \
#     libx11-dev \
#     libxext-dev \
#     libxrandr-dev \
#     libxrender-dev \
#     libbsd-dev \
#     openssl-dev \
#     libffi-dev \
#     xz-dev \
#     bzip2-dev \
#     zsh \
#     clang \
#     clang-dev \
#     alpine-sdk \
#     git \
#     cmake && \
#     [ ! -d "/root/.oh-my-zsh" ] && sh -c "$(curl -fsSL https://raw.githubusercontent.com/ohmyzsh/ohmyzsh/master/tools/install.sh)" || true && \
#     [ ! -d "/root/.oh-my-zsh/custom/plugins/zsh-syntax-highlighting" ] && git clone https://github.com/zsh-users/zsh-syntax-highlighting.git /root/.oh-my-zsh/custom/plugins/zsh-syntax-highlighting || true && \
#     [ ! -d "/root/.oh-my-zsh/custom/plugins/zsh-autosuggestions" ] && git clone https://github.com/zsh-users/zsh-autosuggestions.git /root/.oh-my-zsh/custom/plugins/zsh-autosuggestions || true && \
#     sed -i 's/plugins=(git)/plugins=(git z common-aliases node npm rand-quote sudo colored-man-pages colorize cp zsh-syntax-highlighting zsh-autosuggestions copyfile history dirhistory)/' /root/.zshrc && \
#     ln -sf /usr/bin/clang /usr/bin/cc && \
#     ln -sf /usr/bin/clang++ /usr/bin/c++

# WORKDIR app


#######################
#  OLD UBUNTU VERSION v2
#######################
# FROM ubuntu:latest

# ARG DEBIAN_FRONTEND=noninteractive

# # Set the default shell to zsh
# SHELL ["/bin/zsh", "-c"]

# RUN echo "ipv6" >> /etc/modules && \
#     apt-get update && \
#     apt-get upgrade -y && \
#     apt-get install -y \
#     build-essential \
#     valgrind \
#     libx11-dev \
#     libxext-dev \
#     libxrandr-dev \
#     libxrender-dev \
#     libbsd-dev \
#     libffi-dev \
#     liblzma-dev \
#     libbz2-dev \
#     zsh \
#     clang \
#     clang-format \
#     cmake \
#     git \
#     curl && \
#     [ ! -d "/root/.oh-my-zsh" ] && sh -c "$(curl -fsSL https://raw.githubusercontent.com/ohmyzsh/ohmyzsh/master/tools/install.sh)" || true && \
#     [ ! -d "/root/.oh-my-zsh/custom/plugins/zsh-syntax-highlighting" ] && git clone https://github.com/zsh-users/zsh-syntax-highlighting.git /root/.oh-my-zsh/custom/plugins/zsh-syntax-highlighting || true && \
#     [ ! -d "/root/.oh-my-zsh/custom/plugins/zsh-autosuggestions" ] && git clone https://github.com/zsh-users/zsh-autosuggestions.git /root/.oh-my-zsh/custom/plugins/zsh-autosuggestions || true && \
#     sed -i 's/plugins=(git)/plugins=(git z common-aliases node npm rand-quote sudo colored-man-pages colorize cp zsh-syntax-highlighting zsh-autosuggestions copyfile history dirhistory)/' /root/.zshrc && \
#     ln -sf /usr/bin/clang /usr/bin/cc && \
#     ln -sf /usr/bin/clang++ /usr/bin/c++

# WORKDIR /app

#######################
#  OLD UBUNTU VERSION v1
#######################

# FROM ubuntu:latest

# # Update and install necessary packages
# RUN apt-get update && \
#     apt-get install -y \
#     build-essential \
#     valgrind \
#     libx11-dev \
#     libxext-dev \
#     libxrandr-dev \
#     libxrender-dev \
#     libbsd-dev \
#     libssl-dev \
#     libffi-dev \
#     liblzma-dev \
#     libbz2-dev \
#     zsh \
#     git

# # Set the default shell to zsh
# SHELL ["/bin/zsh", "-c"]

# # Set the working directory
# WORKDIR /app


