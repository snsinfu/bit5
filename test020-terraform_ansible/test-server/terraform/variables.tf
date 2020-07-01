variable "server_hostname" {
  type = string
}

variable "server_location" {
  type    = string
  default = "fsn1"
}

variable "server_type" {
  type    = string
  default = "cx11"
}

variable "server_image" {
  type = string
}

variable "admin_user" {
  type = string
}

variable "admin_ssh_pubkeys" {
  type = list(string)
}
