def sendCMD(self, data_length, code, data):
  checksum = 0
  total_data = ['$', 'M', '<', data_length, code] + data
  for i in struct.pack('<2B%dh' % len(data), *total_data[3:len(total_data)]):
      checksum = checksum ^ ord(i)
  total_data.append(checksum)
  try:
      b = None
      b = self.ser.write(struct.pack('<3c2B%dhB' % len(data), *total_data))
  except Exception, error:
      #print "\n\nError in sendCMD."
      #print "("+str(error)+")\n\n"
      pass
