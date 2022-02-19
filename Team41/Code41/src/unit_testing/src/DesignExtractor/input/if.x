<program>
	<procedure name=main>
		<stmts>
			<if>
				<eq>
					<var name=a></var>
					<const val=1></const>
				</eq>
				<stmts>
					<assign>
						<var name=x></var>
						<const val=1></const>
					</assign>
				</stmts>
				<stmts>
					<assign>
						<var name=xx></var>
						<const val=11></const>
					</assign>
				</stmts>
			</if>
			<if>
				<ne>
					<const val=2></const>
					<var name=b></var>
				</ne>
				<stmts>
					<read>
						<var name=y></var>
					</read>
				</stmts>
				<stmts>
					<read>
						<var name=yy></var>
					</read>
				</stmts>
			</if>
			<if>
				<lt>
					<var name=c></var>
					<var name=d></var>
				</lt>
				<stmts>
					<print>
						<var name=z></var>
					</print>
				</stmts>
				<stmts>
					<print>
						<var name=zz></var>
					</print>
				</stmts>
			</if>
			<if>
				<gt>
					<var name=e></var>
					<const val=3></const>
				</gt>
				<stmts>
					<if>
						<ge>
							<const val=4></const>
							<var name=f></var>
						</ge>
						<stmts>
							<assign>
								<var name=w></var>
								<var name=v></var>
							</assign>
						</stmts>
						<stmts>
							<assign>
								<var name=ww></var>
								<var name=vv></var>
							</assign>
						</stmts>
					</if>
				</stmts>
				<stmts>
					<if>
						<not>
							<le>
								<var name=i></var>
								<const val=5></const>
							</le>
						</not>
						<stmts>
							<assign>
								<var name=r></var>
								<var name=s></var>
							</assign>
						</stmts>
						<stmts>
							<assign>
								<var name=rr></var>
								<var name=ss></var>
							</assign>
						</stmts>
					</if>
				</stmts>
			</if>
			<if>
				<and>
					<le>
						<var name=g></var>
						<const val=5></const>
					</le>
					<not>
						<or>
							<eq>
								<const val=6></const>
								<var name=h></var>
							</eq>
							<ne>
								<const val=7></const>
								<const val=8></const>
							</ne>
						</or>
					</not>
				</and>
				<stmts>
					<assign>
						<var name=t></var>
						<var name=u></var>
					</assign>
				</stmts>
				<stmts>
					<assign>
						<var name=tt></var>
						<var name=uu></var>
					</assign>
				</stmts>
			</if>
		</stmts>
	</procedure>
</program>